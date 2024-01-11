#include <DxLib.h>
#include <string>

#include "Vec2.h"
#include "GameManager.h"
#include "Stage/StageManager.h"
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
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Update(Input& input)
{
	// 上下のインデックスの変更
	if (input.IsTriggered("up"))
	{
		m_indexLine = (m_indexLine - 1 + kLineNum) % kLineNum;
	}
	if (input.IsTriggered("down"))
	{
		m_indexLine = (m_indexLine + 1) % kLineNum;
	}

	// 左右のインデックスの変更
	if (input.IsTriggered("left"))
	{
		m_indexRow = (m_indexRow - 1 + kRowNum) % kRowNum;
	}
	if (input.IsTriggered("right"))
	{
		m_indexRow = (m_indexRow + 1) % kRowNum;
	}

	if (input.IsTriggered("OK"))
	{
		// ステージがないなら終了
		if (kStageStr[m_indexLine][m_indexRow] == "None") return;
		auto stgName = "Stage" + kStageStr[m_indexLine][m_indexRow];

		// そのステージがクリアされていなければ終了
		if (!m_mgr.GetStage()->IsClearStage(stgName)) return;


	}
}

void StageSelectScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"StagSelectScene", 0xffffff);

	for(int x = 0; x < kRowNum; x++)
	{
		int drawX = kStartPos.x + kStageMargine * x;
		bool isIndexRow = (m_indexRow == x);

		for (int y = 0; y < kLineNum; y++)
		{
			if (isIndexRow && m_indexLine == y)
			{
				// FIXE:現状大きめの枠を描画しているがのちは画像に変更
				DrawBoxAA(drawX - kStageFrameSize, kStartPos.y + kStageMargine * y - kStageFrameSize,
					drawX + kStageSize + kStageFrameSize, kStartPos.y + kStageMargine * y + kStageSize + kStageFrameSize,
					0xffff00, false, kStageFrameThickness);
			}

			// ステージの描画が無しの場合は以下のは描画しない
			if (kStageStr[y][x] == "None") continue;

			int drawY = kStartPos.y + kStageMargine * y;

			// 現在選択しているステージ
			// FIXME:仮として円を描画しているので、グラフィックが出来次第変更
			if (("Stage" + kStageStr[y][x]) == m_mgr.GetStage()->GetStageName())
			{
				DrawCircle(drawX + kStageSize * 0.5f, drawY + kStageSize * 0.5f, 16, 0xff8800, true);
			}

			//for (int i = 0; i < kStageFrameThickness; i++)
			//{
			//	DrawBox(drawX + i, kStartPos.y + kStageMargine * y + i,
			//		drawX + kStageSize - i, kStartPos.y + kStageMargine * y + kStageSize - i,
			//		0xffffff, false);
			//}

			// MEMO:アンチエイリアス付きにすると線の太さを指定可能
			DrawBoxAA(drawX, drawY,
				drawX + kStageSize, drawY + kStageSize,
				0xffffff, false, kStageFrameThickness);
		}
	}
}
