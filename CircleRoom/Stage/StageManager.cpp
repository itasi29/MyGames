#include <DxLib.h>
#include <cassert>
#include <cmath>
#include "Application.h"

#include "StageManager.h"
#include "StageBase.h"

namespace
{
	// ファイル用
	struct StageInfHeader
	{
		char id[4] = "sti"; // 最後に'\0'入ってるので4バイト
		float version = 1.0f;
		size_t dataCount = 0;
		// 空白の4バイト(パディング)
	};

	// ステージ間の移動にかかる時間
	constexpr int kStageMoveFrame = 30;
}

StageManager::StageManager() :
	m_frame(0),
	m_isStageMove(false),
	m_stageHandle(-1),
	m_size(Application::GetInstance().GetWindowSize())
{
	// ステージ1-1に二つ情報追加
	m_stageClearTable["1-1"].push_back(false);
	m_stageClearTable["1-1"].push_back(false);
	// ステージ1-2に二つ情報追加
	m_stageClearTable["1-2"].push_back(false);
	// ステージ1-3に二つ情報追加
	m_stageClearTable["1-2"].push_back(false);

	Load(L"stg.inf");
}

StageManager::~StageManager()
{
	Save("stg.inf");
}

void StageManager::Update(Input& input)
{
	UpdateMove();
	m_stage->Update(input);
	m_stage->ChangeStage(input);
}

void StageManager::Draw()
{
	if (m_isStageMove)
	{
		DrawMove();
	}
	else
	{
		m_stage->Draw();
	}
}

void StageManager::ChangeStage(std::shared_ptr<StageBase> nextStage)
{
	m_stage = nextStage;
}

void StageManager::StartMove(MoveDir dir, int handle)
{
	m_isStageMove = true;
	m_frame = 0;

	switch (dir)
	{
	case StageManager::kDirLeft:
		// 位置を調整する
		// 除算の割る方を+1しているのは1280まで余りを出せるようにするため
		m_pos.x = static_cast<float>((static_cast<int>(m_pos.x) + m_size.w) % (m_size.w + 1));

		m_vec.x = (0 - m_pos.x) / kStageMoveFrame;
		ResetVecY();
		break;
	case StageManager::kDirRight:
		// こちらは位置を調整しない

		m_vec.x = static_cast<float>((m_size.w - static_cast<int>(m_pos.x)) % (m_size.w + 1) / kStageMoveFrame);
		ResetVecY();
		break;
	case StageManager::kDirUp:
		m_pos.y = static_cast<float>((static_cast<int>(m_pos.y) + m_size.h) % (m_size.h + 1));
		
		m_vec.y = (0 - m_pos.y) / kStageMoveFrame;
		ResetVecX();
		break;
	case StageManager::kDirDown:

		m_vec.y = static_cast<float>((m_size.h - static_cast<int>(m_pos.y)) % (m_size.h + 1) / kStageMoveFrame);
		ResetVecX();
		break;
	default:
		assert(false);
		break;
	}

	// 中身が入っていたらそれを消す
	if (m_stageHandle != 0)
	{
		DeleteGraph(m_stageHandle);
	}
	m_stageHandle = handle;
}

int StageManager::GetSlideVolumeX(MoveDir dir) const
{
	if (dir == kDirRight && m_vec.x > 0.0f)
	{
		return static_cast<int>(m_size.w);
	}
	if (dir == kDirLeft && m_vec.x < 0.0f)
	{
		return static_cast<int>(m_size.w);
	}

	return 0;
}

int StageManager::GetSlideVolumeY(MoveDir dir) const
{
	if (dir == kDirDown && m_vec.y > 0.0f)
	{
		return static_cast<int>(m_size.h);
	}
	if (dir == kDirUp && m_vec.y < 0.0f)
	{
		return static_cast<int>(m_size.h);
	}

	return 0;
}

void StageManager::UpdateMove()
{
	if (!m_isStageMove) return;

	// フレームの更新
	m_frame++;
	// 場所の更新
	m_pos += m_vec;

	// 一定フレームたったら動かし完了とする
	if (m_frame >= kStageMoveFrame)
	{
		m_isStageMove = false;

		// 位置、ベクトルも初期化しておく
		m_pos = { 0.0f, 0.0f };
		m_vec = { 0.0f, 0.0f };
	}
}

void StageManager::DrawMove()
{
	DrawRectGraph(0, 0, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_size.w, m_size.h,
		m_stageHandle, true);

#ifdef _DEBUG
	DrawFormatString(32, 32, 0xff0808, L"ステージ移動中 %d", m_frame);
	DrawFormatString(32, 48, 0xff0808, L"座標(%.2f, %.2f)", m_pos.x, m_pos.y);
#endif
}

void StageManager::ResetVecX()
{
	// 左に動いている
	if (m_vec.x < 0.0f)
	{
		m_vec.x = -m_pos.x / kStageMoveFrame;
		return;
	}
	// 右に動いている
	if (m_vec.x > 0.0f)
	{
		m_vec.x = static_cast<float>((m_size.w - static_cast<int>(m_pos.x)) % (m_size.w + 1) / kStageMoveFrame);
		return;
	}
}

void StageManager::ResetVecY()
{
	// 上に動いている
	if (m_vec.y < 0.0f)
	{
		m_vec.y = -m_pos.y / kStageMoveFrame;
		return;
	}
	// 下に動いている
	if (m_vec.y > 0.0f)
	{
		m_vec.y = static_cast<float>((m_size.h - static_cast<int>(m_pos.y)) % (m_size.h + 1) / kStageMoveFrame);
		return;
	}
}

void StageManager::Save(const std::string& path)
{
	FILE* fp = nullptr; // ファイルポインタ
	auto err = fopen_s(&fp, path.c_str(), "wb");
	//if (err == errno)
	//{
	//	// 読み込みに失敗したため終了
	//	assert(false);
	//	return;
	//}
	// ヘッダの書き込み
	StageInfHeader header;
	header.dataCount = m_stageClearTable.size();
	fwrite(&header, sizeof(header), 1, fp);

	// データ本体を書き込んでいく
	for (const auto& stg : m_stageClearTable)
	{
		const auto& stageStr = stg.first; // ステージ名文字列
		uint8_t size = stageStr.size();   // ステージ名文字列のサイズを取得
		fwrite(&size, sizeof(size), 1, fp); // ステージ名文字列のバイト数を書き込む
		fwrite(stageStr.data(), stageStr.size(), 1, fp);    // 文字列の書き込み

		uint8_t vectorSize = stg.second.size();	// 配列の数を取得
		fwrite(&vectorSize, sizeof(vectorSize), 1, fp);	// 配列数を書き込み
		for (const auto& inf : stg.second)
		{
			fwrite(&inf, sizeof(inf), 1, fp);	// クリア情報を書き込む
		}
	}

	fclose(fp);
}

void StageManager::Load(const std::wstring& path)
{
	auto handle = FileRead_open(path.c_str());
	// エラー(ファイルがない)場合は処理しない
	if (handle == 0)
	{
		return;
	}
	// ヘッダ情報の読み込み
	StageInfHeader header;
	FileRead_read(&header, sizeof(header), handle);

	// データの読み込み
	for (int i = 0; i < header.dataCount; i++)
	{
		// ステージ名文字列サイズを入れる変数
		uint8_t stgStrSize = 0;
		// ステージ名文字列サイズ読み込む
		FileRead_read(&stgStrSize, sizeof(stgStrSize), handle);
		// ステージ名文字列を入れる変数
		std::string stgStr;
		// ステージ名文字列サイズでリサイズ
		stgStr.resize(stgStrSize);
		// ステージ名読み込み
		FileRead_read(stgStr.data(), stgStr.size() * sizeof(char), handle);

		// ステージクリアテーブルから情報群のvectorの参照を取得
		auto& infVec = m_stageClearTable[stgStr];

		// vectorのサイズを入れる変数
		uint8_t vectorSize;
		// vectroのサイズを取得する
		FileRead_read(&vectorSize, sizeof(vectorSize), handle);

		// vectorのサイズでリサイズする
		infVec.resize(vectorSize);

		for (int j = 0; j < vectorSize; j++)
		{
			// ブールの情報を取得する
			bool inf;

			FileRead_read(&inf, sizeof(inf), handle);

			infVec[j] = inf;
		}
	}
}

void StageManager::SaveClearInf(const std::string& stgName, const std::vector<bool>& inf)
{
	auto& data = m_stageClearTable[stgName];

	data.resize(inf.size());

	for (int i = 0; i < inf.size(); i++)
	{
		data[i] = inf[i];
	}
}

bool StageManager::GetClearInf(const std::string& stgName, std::vector<bool>& inf)
{
	auto it = m_stageClearTable.find(stgName);
	// 要素がない場合はfalseを返す
	if (it == m_stageClearTable.end())
	{
		assert(false);
		return false;
	}

	const auto& data = m_stageClearTable[stgName];

	inf.resize(data.size());

	for (int i = 0; i < data.size(); i++)
	{
		inf[i] = data[i];
	}
	// ここまで来たら保存できた
	return true;
}
