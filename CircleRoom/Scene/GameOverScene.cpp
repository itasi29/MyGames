#include <DxLib.h>
#include <cassert>
#include "../Input.h"
#include "SceneManager.h"
#include "GameOverScene.h"
#include "TitleScene.h"

GameOverScene::GameOverScene(SceneManager& mgr) :
	Scene(mgr)
{
	m_handle = LoadGraph(L"./Image/gameOver.png");
	assert(m_handle >= 0);
	m_frame = 60;
	m_updateFunc = &GameOverScene::FadeInUpdate;
	m_drawFunc = &GameOverScene::FadeDraw;
}

GameOverScene::~GameOverScene()
{
	DeleteGraph(m_handle);
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
		m_manager.ChangeScene(std::make_shared<TitleScene>(m_manager));
	}
}

void GameOverScene::FadeDraw()
{
	DrawString(10, 100, L"GameOverScene", 0xffffff);
	DrawGraph(100, 100, m_handle, true);

	int alpha = 255 * (static_cast<float>(m_frame) / 60.0f);
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameOverScene::NormalDraw()
{
	DrawString(10, 100, L"GameOverScene", 0xffffff);
	DrawGraph(100, 100, m_handle, true);
}
