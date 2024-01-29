#include <DxLib.h>
#include <string>

#include "StringUtility.h"
#include "Vec2.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "Input.h"

#include "Stage/Stage1_1.h"
#include "Stage/Stage1_2.h"
#include "Stage/Stage1_3.h"
#include "Stage/Stage1_4.h"
#include "Stage/Stage1_5.h"
#include "Stage/Stage1_6.h"
#include "Stage/Stage1_7.h"
#include "Stage/Stage1_8.h"
#include "Stage/Stage1_9.h"

#include "StageSelectScene.h"

namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// 強調文字列の色
	constexpr unsigned int kYellowColor = 0xffde00;

	constexpr int kStageNum = 5;

	// ステージ枠の大きさ
	constexpr int kStageSize = 64;
	// ステージ枠とフレームの差
	// MEMO:後で画像に変更するためおそらく消す
	constexpr int kStageFrameSize = 14;
	// フレームの太さ
	constexpr int kStageFrameThickness = 4;
	// ステージ間の空白
	constexpr int kStageMargine = 128;

	// スタート位置
	constexpr int kStartX = 400;
	constexpr int kStartY = 216;

	// 情報文字列描画位置
	constexpr int kDrawStringX = 200;
	constexpr int kDrawStringY = 216;

	// ステージの縦横の数
	constexpr int kLineNum = 3;
	constexpr int kRowNum = 3;
	// ステージ名簿
	const std::string kStageStr[kLineNum][kRowNum] =
	{
		{"Stage1-5", "Stage1-8", "Stage1-7"},
		{"Stage1-3", "Stage1-4", "Stage1-6"},
		{"Stage1-2", "Stage1-1", "Stage1-9"}
	};
}

StageSelectScene::StageSelectScene(GameManager& mgr) :
	Scene(mgr),
	m_indexRow(0),
	m_indexLine(0),
	m_fadeFrame(0)
{
	CurrosrPos();
	m_soundSys = mgr.GetSound();

	auto& file = mgr.GetFile();
	m_frame = file->LoadGraphic(L"UI/selectFrame.png");
	m_nowPos = file->LoadGraphic(L"UI/nowPos.png");
	m_lock = file->LoadGraphic(L"UI/lock.png");

	m_selectSe = file->LoadSound(L"Se/select.mp3");
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Update(Input& input)
{
	m_fadeFrame++;

	// 上下のインデックスの変更
	if (input.IsTriggered("up"))
	{
		m_indexLine = (m_indexLine - 1 + kLineNum) % kLineNum;
		m_fadeFrame = 0;
	}
	if (input.IsTriggered("down"))
	{
		m_indexLine = (m_indexLine + 1) % kLineNum;
		m_fadeFrame = 0;
	}

	// 左右のインデックスの変更
	if (input.IsTriggered("left"))
	{
		m_indexRow = (m_indexRow - 1 + kRowNum) % kRowNum;
		m_fadeFrame = 0;
	}
	if (input.IsTriggered("right"))
	{
		m_indexRow = (m_indexRow + 1) % kRowNum;
		m_fadeFrame = 0;
	}

	if (input.IsTriggered("OK"))
	{
		// ステージがないなら終了
		if (kStageStr[m_indexLine][m_indexRow] == "None") return;
		auto stgName = kStageStr[m_indexLine][m_indexRow];

		// そのステージがクリアされていなければ終了
		if (!m_mgr.GetStage()->IsClearStage(stgName)) return;

		m_soundSys->PlaySe(m_selectSe->GetHandle());

		if (stgName == "Stage1-1")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_1>(m_mgr, input));
		}
		if (stgName == "Stage1-2")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_2>(m_mgr, input));
		}
		if (stgName == "Stage1-3")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_3>(m_mgr, input));
		}
		if (stgName == "Stage1-4")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_4>(m_mgr, input));
		}
		if (stgName == "Stage1-5")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_5>(m_mgr, input));
		}
		if (stgName == "Stage1-6")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_6>(m_mgr, input));
		}
		if (stgName == "Stage1-7")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_7>(m_mgr, input));
		}
		if (stgName == "Stage1-8")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_8>(m_mgr, input));
		}
		if (stgName == "Stage1-9")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_9>(m_mgr, input));
		}

		m_mgr.GetScene()->PopScene();
	}
}

void StageSelectScene::Draw()
{
	for(int x = 0; x < kRowNum; x++)
	{
		int drawX = static_cast<int>(kStageMargine * x) + kStartX;
		bool isIndexRow = (m_indexRow == x);

		for (int y = 0; y < kLineNum; y++)
		{
			// 現在選択しているステージ
			const std::string& stageName = kStageStr[y][x];
			// フレームの描画
			if (isIndexRow && m_indexLine == y)
			{
				int frame = (m_fadeFrame % 80) - 40;
				float rate = fabsf(static_cast<float>(frame)) / 40.0f;
				int alpha = static_cast<int>(255 * rate);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
				DrawGraph(drawX - kStageFrameSize, kStartY + kStageMargine * y - kStageFrameSize, m_frame->GetHandle(), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


				// 情報の描画
				DrawInf(stageName);
			}

			// ステージの描画が無しの場合は以下のは描画しない
			if (kStageStr[y][x] == "None") continue;

			int drawY = static_cast<int>(kStartY + kStageMargine * y);

			if ((stageName) == m_mgr.GetStage()->GetStageName())
			{
				DrawRotaGraph(static_cast<int>(drawX + kStageSize * 0.5f), static_cast<int>(drawY + kStageSize * 0.5f),
					1.0, 0.0, m_nowPos->GetHandle(), true);

			}

			// ステージの描画
			// MEMO:アンチエイリアス付きにすると線の太さを指定可能
			if (m_mgr.GetStage()->IsClearStage(stageName))
			{
				DrawBoxAA(static_cast<float>(drawX), static_cast<float>(drawY),
					static_cast<float>(drawX + kStageSize), static_cast<float>(drawY + kStageSize),
					0xffffff, false, kStageFrameThickness);
			}
			else
			{
				DrawBoxAA(static_cast<float>(drawX), static_cast<float>(drawY),
					static_cast<float>(drawX + kStageSize), static_cast<float>(drawY + kStageSize),
					0xff0000, false, kStageFrameThickness);

				DrawGraph(drawX + 16, drawY + 12, m_lock->GetHandle(), true);
			}
		}
	}
}

void StageSelectScene::DrawInf(const std::string& str)
{
	int font = m_mgr.GetFont()->GetHandle(32);

	const auto& wstr = StringUtility::StringToWString(str);

	int y = kDrawStringY;

	// ステージ名の描画
	DrawStringToHandle(kDrawStringX, y, wstr.c_str(), kWhiteColor, font);
	// ベストタイムの描画
	y += 96;
	DrawStringToHandle(kDrawStringX, y, L"ベストタイム", kWhiteColor, font);
	y += 48;

	int bestTime = m_mgr.GetStage()->GetBestTime(str);
	int minSec = (bestTime * 1000 / 60) % 1000;
	int sec = (bestTime / 60) % 60;
	int min = bestTime / 3600;
	DrawFormatStringToHandle(kDrawStringX, y, kYellowColor, font, L"%02d:%02d.%03d", min, sec, minSec);

}

void StageSelectScene::CurrosrPos()
{
	const auto& nowStage = m_mgr.GetNowStage();

	for (int x = 0; x < kRowNum; x++)
	{
		for (int y = 0; y < kLineNum; y++)
		{
			if (nowStage == kStageStr[y][x])
			{
				m_indexRow = x;
				m_indexLine = y;

				return;
			}
		}
	}
}
