#include <DxLib.h>
#include <cassert>
#include <string>
#include "Common/Input.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FontSystem.h"
#include "GameClearScene.h"
#include "TitleScene.h"

namespace
{
	constexpr int kCreditsNum = 3;

	constexpr int kTextInterval = 180;

	const std::wstring kCredits[kCreditsNum] = {
		L"クリア時間",
		L"死亡数",
		L"ダッシュ使用回数"
	};
}

GameClearScene::GameClearScene(GameManager& mgr) :
	Scene(mgr),
	m_frame(60),
	m_textFrame(0),
	m_index(0)
{
	m_updateFunc = &GameClearScene::FadeInUpdate;
	m_drawFunc = &GameClearScene::FadeDraw;
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void GameClearScene::Draw()
{
	(this->*m_drawFunc)();
}

void GameClearScene::FadeInUpdate(Input&)
{
	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &GameClearScene::NormalUpdate;
		m_drawFunc = &GameClearScene::NormalDraw;

		m_frame = 0;
	}
}

void GameClearScene::NormalUpdate(Input& input)
{
	m_textFrame++;

	if (m_index < kCreditsNum)
	{
		if (m_textFrame > kTextInterval || input.IsTriggered("OK"))
		{
			m_textFrame = 0;
			m_index++;
		}

		return;
	}

	if (input.IsAnyTriggerd())
	{
		m_updateFunc = &GameClearScene::FadeOutUpdate;
		m_drawFunc = &GameClearScene::FadeDraw;

		m_frame = 0;
	}
}

void GameClearScene::FadeOutUpdate(Input&)
{
	m_frame++;
	if (60 <= m_frame)
	{
		m_mgr.GetScene()->ChangeScene(std::make_shared<TitleScene>(m_mgr));
	}
}

void GameClearScene::FadeDraw()
{
	NormalDraw();

	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameClearScene::NormalDraw()
{
	DrawString(100, 100, L"GameClear", 0xffffff);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	int drawY = 320;
	for (int i = 0; i < m_index + 1; i++)
	{
		if (!(i < kCreditsNum))
		{
			int val = (m_textFrame + kTextInterval) % (kTextInterval * 2) - kTextInterval;
			float rate = (fabs(static_cast<float>(val)) / kTextInterval);
			int alpha = static_cast <int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(500, drawY, L"〜　AnyBottanPush　〜", 0xffffff, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		}

		if (i == m_index)
		{
			float rate = (static_cast<float>(m_textFrame) / kTextInterval);
			int alpha = static_cast <int>(255 * rate);
			int y = drawY - static_cast<int>(100 * (1 - rate));

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawFormatStringToHandle(500, y, 0xffffff, fontHandle, L"%s", kCredits[i].c_str());
			DrawInf(i, y, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawFormatStringToHandle(500, drawY, 0xffffff, fontHandle, L"%s", kCredits[i].c_str());
			DrawInf(i, drawY, fontHandle);
		}

		drawY += 100;
	}
}

void GameClearScene::DrawInf(int index, int drawY, int handle)
{
	const auto& data = m_mgr.GetData();

	if (index == 0)
	{
		int sec = (data.playTime / 60) % 60;
		int min = (data.playTime / 3600) % 60;
		int hour = (data.playTime / 21600);
		DrawFormatStringToHandle(800, drawY, 0xffffff, handle, L"%02d:%02d:%02d", hour, min, sec);
	}
	else if (index == 1)
	{
		DrawFormatStringToHandle(800, drawY, 0xffffff, handle, L"%-3d回", data.deathCount);
	}
	else
	{
		DrawFormatStringToHandle(800, drawY, 0xffffff, handle, L"%-3d回", data.dashCount);
	}
}
