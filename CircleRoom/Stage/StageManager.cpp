#include <DxLib.h>
#include <cassert>
#include <cmath>
#include "Application.h"

#include "StageManager.h"
#include "StageBase.h"

// ファイル用
struct StageInfHeader
{
	char id[4] = "sti"; // 最後に'\0'入ってるので4バイト
	float version = 1.0f;
	size_t dataCount = 0;
	// 空白の4バイト(パディング)
};

namespace
{
	// ステージ間の移動にかかる時間
	constexpr int kStageMoveFrame = 30;
}

StageManager::StageManager() :
	m_frame(0),
	m_isStageMove(false),
	m_stageHandle(-1),
	m_size(Application::GetInstance().GetWindowSize())
{
	// 初期化(読み込みに失敗した場合は初プレイと同じとする)
	m_stageSaveData.clear();
	m_killedEnemyNameTable.clear();
	m_killedEnemyCount = 0;

	Load(L"stg.inf");
}

StageManager::~StageManager()
{
	Save("stg.inf");
}

void StageManager::DeleteData()
{
	m_stageSaveData.clear();
	m_killedEnemyNameTable.clear();
	m_killedEnemyCount = 0;
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

void StageManager::StartMove(StageManager::StageDir dir, int handle)
{
	m_isStageMove = true;
	m_frame = 0;

	switch (dir)
	{
	case StageManager::kStageLeft:
		// 位置を調整する
		// 除算の割る方を+1しているのは1280まで余りを出せるようにするため
		m_pos.x = static_cast<float>((static_cast<int>(m_pos.x) + m_size.w) % (m_size.w + 1));

		m_vec.x = (0 - m_pos.x) / kStageMoveFrame;
		ResetVecY();
		break;
	case StageManager::kStageRight:
		// こちらは位置を調整しない

		m_vec.x = static_cast<float>((m_size.w - static_cast<int>(m_pos.x)) % (m_size.w + 1) / kStageMoveFrame);
		ResetVecY();
		break;
	case StageManager::kStageUp:
		m_pos.y = static_cast<float>((static_cast<int>(m_pos.y) + m_size.h) % (m_size.h + 1));
		
		m_vec.y = (0 - m_pos.y) / kStageMoveFrame;
		ResetVecX();
		break;
	case StageManager::kStageDown:

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

int StageManager::GetSlideVolumeX(StageDir dir) const
{
	if (dir == kStageRight && m_vec.x > 0.0f)
	{
		return static_cast<int>(m_size.w);
	}
	if (dir == kStageLeft && m_vec.x < 0.0f)
	{
		return static_cast<int>(m_size.w);
	}

	return 0;
}

int StageManager::GetSlideVolumeY(StageDir dir) const
{
	if (dir == kStageDown && m_vec.y > 0.0f)
	{
		return static_cast<int>(m_size.h);
	}
	if (dir == kStageUp && m_vec.y < 0.0f)
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
	if (err != errno)
	{
		// 読み込みに失敗したため終了
		assert(false);
		return;
	}
	// ヘッダの書き込み
	StageInfHeader header;
	header.dataCount = m_stageSaveData.size();
	fwrite(&header, sizeof(header), 1, fp);

	// クリアデータの書き込み
	for (const auto& stage : m_stageSaveData)
	{
		// ステージ名文字列のサイズを取得
		const auto& stageStr = stage.first; 
		uint8_t size = static_cast<uint8_t>(stageStr.size());
		// ステージ名文字列のバイト数を書き込む
		fwrite(&size, sizeof(size), 1, fp); 
		// 文字列の書き込み
		fwrite(stageStr.data(), stageStr.size(), 1, fp);    

		// データ群の参照
		auto& data = stage.second;
		// ベストタイムの書き込み
		fwrite(&data.bestTime, sizeof(data.bestTime), 1, fp);
		// 配列数を書き込み
		uint8_t dataSize = static_cast<uint8_t>(data.isClears.size());
		fwrite(&dataSize, sizeof(dataSize), 1, fp);
		for (const auto& isClear : data.isClears)
		{
			// クリア情報を書き込む
			fwrite(&isClear, sizeof(isClear), 1, fp);
		}
	}

	// データ本体を書き込んでいく
	// 殺された敵の種類数の書き込み
	fwrite(&m_killedEnemyCount, sizeof(m_killedEnemyCount), 1, fp);
	// 名前データの書き込み
	for (const auto& name : m_killedEnemyNameTable)
	{
		// 名前の文字列数を取得
		uint8_t nameSize = static_cast<uint8_t>(name.size());
		// 文字列数を書き込み
		fwrite(&nameSize, sizeof(nameSize), 1, fp);
		// 文字列の書き込み
		fwrite(name.data(), name.size(), 1, fp);
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
		FileRead_read(stgStr.data(), static_cast<int>(stgStr.size() * sizeof(char)), handle);

		// ステージクリアテーブルから情報群のvector<StageData>の参照を取得
		auto& data = m_stageSaveData[stgStr];

		// ベストタイム読み込み
		FileRead_read(&data.bestTime, sizeof(data.bestTime), handle);

		// 配列数読み込み
		uint8_t dataSize;
		FileRead_read(&dataSize, sizeof(dataSize), handle);

		// vectorのリサイズ
		data.isClears.resize(dataSize);
		for (int j = 0; j < dataSize; j++)
		{
			// クリア情報の取得
			bool isClear;

			FileRead_read(&isClear, sizeof(isClear), handle);

			data.isClears[j] = isClear;
		}
	}

	// 殺された敵の種類数の取得
	FileRead_read(&m_killedEnemyCount, sizeof(m_killedEnemyCount), handle);
	// 名前情報群の数を種類数でリサイズ
	m_killedEnemyNameTable.resize(m_killedEnemyCount);
	for (int i = 0; i < m_killedEnemyCount; i++)
	{
		// 名前文字列サイズを入れる変数
		uint8_t nameSize;
		// サイズの読み込み
		FileRead_read(&nameSize, sizeof(nameSize), handle);

		auto& name = m_killedEnemyNameTable[i];
		// サイズで文字列をリサイズ
		name.resize(nameSize);
		// 文字列の読み込み
		FileRead_read(name.data(), static_cast<int>(name.size() * sizeof(char)), handle);
	}

	// ファイルは閉じる
	FileRead_close(handle);
}

void StageManager::CreateData(const std::string& stgName)
{
	auto it = m_stageSaveData.find(stgName);
	// ステージを見つけたら何もしない
	if (it != m_stageSaveData.end())
	{
		return;
	}

	// なければ作成する
	auto& data = m_stageSaveData[stgName];

	// 情報の初期化
	data.bestTime = 0;
	data.isClears.resize(kStageMax);
	for (int i = 0; i < data.isClears.size(); i++)
	{
		data.isClears[i] = false;
	}
}

bool StageManager::IsClear(const std::string& stgName, StageDir dir) const
{
	auto it = m_stageSaveData.find(stgName);
	// ステージを見つけられなかったら0を返す
	if (it == m_stageSaveData.end())
	{
		assert(false);
		return false;
	}

	// 見つかったらクリア情報を返す
	return m_stageSaveData.at(stgName).isClears[dir];
}

void StageManager::SaveClear(const std::string& stgName, int dir)
{
	auto it = m_stageSaveData.find(stgName);
	// ステージを見つけられなかったら何もしない
	if (it == m_stageSaveData.end())
	{
		assert(false);
		return;
	}

	// 指定のものをクリアとする
	m_stageSaveData[stgName].isClears[dir] = true;
}

int StageManager::GetBestTime(const std::string& stgName) const
{
	auto it = m_stageSaveData.find(stgName);
	// ステージを見つけられなかったら0を返す
	if (it == m_stageSaveData.end())
	{
		assert(false);
		return 0;
	}

	// 見つかったらベストタイムを返す
	return m_stageSaveData.at(stgName).bestTime;
}

void StageManager::UpdateBestTime(const std::string& stgName, int bestTime)
{
	auto it = m_stageSaveData.find(stgName);
	// ステージを見つけられなかったら何もしない
	if (it == m_stageSaveData.end())
	{
		assert(false);
		return;
	}

	auto& time = m_stageSaveData[stgName].bestTime;

	// 現在保存されているタイムが更新タイムより大きければ更新は行わない
	if (time > bestTime)
	{
		return;
	}

	// タイムの更新
	time = bestTime;
}

int StageManager::GetKilledEnemyCount() const
{
	return m_killedEnemyCount;
}

void StageManager::UpdateKilledEnemy(std::string name)
{
	// 配列全部繰り返す
	for (auto& tableName : m_killedEnemyNameTable)
	{
		// 現在のテーブルの名前と殺した敵の名前が同じ場合は一度殺したことがあるから
		// この処理を終了する
		if (tableName == name)
		{
			return;
		}
	}

	// ここまで来たら殺したことがなかった敵である
	// 種類数カウントの増加
	m_killedEnemyCount++;
	// テーブルに登録
	m_killedEnemyNameTable.push_back(name);
}
