#include <DxLib.h>
#include <string>

#include "Vec2.h"
#include "GameManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FileBase.h"
#include "Input.h"

#include "Stage/Stage1_1.h"
#include "Stage/Stage1_2.h"
#include "Stage/Stage1_3.h"
#include "Stage/Stage1_4.h"
#include "Stage/Stage1_5.h"

#include "StageSelectScene.h"

namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

	constexpr int kStageNum = 5;

	// ステージ枠の大きさ
	constexpr int kStageSize = 64;
	// ステージ枠とフレームの差
	// MEMO:後で画像に変更するためおそらく消す
	constexpr int kStageFrameSize = 16;
	// フレームの太さ
	constexpr int kStageFrameThickness = 4;
	// ステージ間の空白
	constexpr int kStageMargine = 96;

	// スタート位置
	const Vec2 kStartPos(kMenuMargin + kStageMargine, kMenuMargin + kStageMargine);

	// ステージの縦横の数
	constexpr int kLineNum = 3;
	constexpr int kRowNum = 2;
	// ステージ名簿
	const std::string kStageStr[kLineNum][kRowNum] = {
		{"1-5", "None"},
		{"1-4", "1-3"},
		{"1-2", "1-1"}
	};
}

StageSelectScene::StageSelectScene(GameManager& mgr) :
	Scene(mgr),
	m_indexRow(0),
	m_indexLine(0)
{
	m_soundSys = mgr.GetSound();
	m_selectSe = mgr.GetFile()->LoadSound(L"Se/select.mp3");
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Update(Input& input)
{
	m_frame++;

	// 上下のインデックスの変更
	if (input.IsTriggered("up"))
	{
		m_indexLine = (m_indexLine - 1 + kLineNum) % kLineNum;
		m_frame = 0;
	}
	if (input.IsTriggered("down"))
	{
		m_indexLine = (m_indexLine + 1) % kLineNum;
		m_frame = 0;
	}

	// 左右のインデックスの変更
	if (input.IsTriggered("left"))
	{
		m_indexRow = (m_indexRow - 1 + kRowNum) % kRowNum;
		m_frame = 0;
	}
	if (input.IsTriggered("right"))
	{
		m_indexRow = (m_indexRow + 1) % kRowNum;
		m_frame = 0;
	}

	if (input.IsTriggered("OK"))
	{
		// ステージがないなら終了
		if (kStageStr[m_indexLine][m_indexRow] == "None") return;
		auto stgName = "Stage" + kStageStr[m_indexLine][m_indexRow];

		// そのステージがクリアされていなければ終了
		if (!m_mgr.GetStage()->IsClearStage(stgName)) return;

		m_soundSys->PlaySe(m_selectSe->GetHandle());

		if (stgName == "Stage1-1")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_1>(m_mgr));
		}
		if (stgName == "Stage1-2")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_2>(m_mgr));
		}
		if (stgName == "Stage1-3")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_3>(m_mgr));
		}
		if (stgName == "Stage1-4")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_4>(m_mgr));
		}
		if (stgName == "Stage1-5")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_5>(m_mgr));
		}
	}
}

void StageSelectScene::Draw()
{
#ifdef _DEBUG
	DrawString(100, kMenuMargin + 10, L"StagSelectScene", 0xffffff);
#endif

	for(int x = 0; x < kRowNum; x++)
	{
		int drawX = static_cast<int>(kStartPos.x + kStageMargine * x);
		bool isIndexRow = (m_indexRow == x);

		for (int y = 0; y < kLineNum; y++)
		{
			// フレームの描画
			if (isIndexRow && m_indexLine == y)
			{
				int frame = (m_frame % 80) - 40;
				float rate = fabs(frame) / 40.0f;
				int alpha = 255 * rate;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
				// FIXE:現状大きめの枠を描画しているがのちは画像に変更
				DrawBoxAA(static_cast<float>(drawX - kStageFrameSize), kStartPos.y + kStageMargine * y - kStageFrameSize,
					static_cast<float>(drawX + kStageSize + kStageFrameSize), kStartPos.y + kStageMargine * y + kStageSize + kStageFrameSize,
					0xffff00, false, kStageFrameThickness);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			// ステージの描画が無しの場合は以下のは描画しない
			if (kStageStr[y][x] == "None") continue;

			int drawY = static_cast<int>(kStartPos.y + kStageMargine * y);

			// 現在選択しているステージ
			// FIXME:仮として円を描画しているので、グラフィックが出来次第変更

			std::string stageName = "Stage" + kStageStr[y][x];
			if ((stageName) == m_mgr.GetStage()->GetStageName())
			{
				DrawCircle(static_cast<int>(drawX + kStageSize * 0.5f), static_cast<int>(drawY + kStageSize * 0.5f), 16, 0xff8800, true);
			}

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
			}
		}
	}
}
