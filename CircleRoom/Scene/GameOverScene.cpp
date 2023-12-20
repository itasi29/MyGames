#include <DxLib.h>
#include <cassert>
#include "Common/Input.h"
#include "GameManager.h"
#include "GameOverScene.h"
#include "TitleScene.h"

GameOverScene::GameOverScene(GameManager& mgr) :
	Scene(mgr)
{
	m_frame = 60;
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
	}
}

void GameOverScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_updateFunc = &GameOverScene::FadeOutUpdate;
		m_drawFunc = &GameOverScene::FadeDraw;
	}
}

void GameOverScene::FadeOutUpdate(Input&)
{
	m_frame++;
	if (60 <= m_frame)
	{
		m_mgr.GetScene().ChangeScene(std::make_shared<TitleScene>(m_mgr));
	}
}

void GameOverScene::FadeDraw()
{
	DrawString(600, 320, L"GameClear", 0xffffff);

	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameOverScene::NormalDraw()
{
	DrawString(600, 320, L"GameClear", 0xffffff);
}
