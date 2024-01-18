#include <DxLib.h>
#include <cassert>
#include <string>
#include "Common/Input.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FontSystem.h"
#include "GameOverScene.h"
#include "TitleScene.h"

namespace
{ 
	constexpr int kCreditsNum = 3;

	constexpr int kTextInterval = 180;

	const std::wstring kCredits[kCreditsNum] = {
		L"Ž€–S”",
		L"ƒNƒŠƒAŽžŠÔ",
		L"–¼‘O"
	};
}

GameOverScene::GameOverScene(GameManager& mgr) :
	Scene(mgr),
	m_frame(60),
	m_textFrame(0),
	m_index(0)
{
	m_updateFunc = &GameOverScene::FadeInUpdate;
	m_drawFunc = &GameOverScene::FadeDraw;
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void GameOverScene::Draw()
{
	(this->*m_drawFunc)();
}

void GameOverScene::FadeInUpdate(Input&)
{
	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &GameOverScene::NormalUpdate;
		m_drawFunc = &GameOverScene::NormalDraw;

		m_frame = 0;
	}
}

void GameOverScene::NormalUpdate(Input& input)
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

	if (input.IsTriggered("OK"))
	{
		m_updateFunc = &GameOverScene::FadeOutUpdate;
		m_drawFunc = &GameOverScene::FadeDraw;

		m_frame = 0;
	}
}

void GameOverScene::FadeOutUpdate(Input&)
{
	m_frame++;
	if (60 <= m_frame)
	{
		m_mgr.GetScene()->ChangeScene(std::make_shared<TitleScene>(m_mgr));
	}
}

void GameOverScene::FadeDraw()
{
	NormalDraw();

	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameOverScene::NormalDraw()
{
	DrawString(100, 100, L"GameClear", 0xffffff);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	int drawY = 320;
	for (int i = 0; i < m_index + 1; i++)
	{
		if (i == m_index)
		{
			float rate = (static_cast<float>(m_textFrame) / kTextInterval);
			int alpha = 255 * rate;
			int y = drawY - (100 * (1 - rate));

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawFormatStringToHandle(600, y, 0xffffff, fontHandle, L"%s", kCredits[i].c_str());
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawFormatStringToHandle(600, drawY, 0xffffff, fontHandle, L"%s", kCredits[i].c_str());
		}

		drawY += 100;
	}
}
