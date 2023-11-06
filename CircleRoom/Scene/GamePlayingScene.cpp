#include <DxLib.h>
#include <cassert>
#include "../Input.h"
#include "SceneManager.h"
#include "GamePlayingScene.h"
#include "GameOverScene.h"
#include "PauseScene.h"

GamePlayingScene::GamePlayingScene(SceneManager& manager) :
	Scene(manager)
{
	m_handle = LoadGraph(L"./Image/game.png");
	assert(m_handle >= 0);
	m_frame = 60;
	m_updateFunc = &GamePlayingScene::FadeInUpdate;
	m_drawFunc = &GamePlayingScene::FadeDraw;
}

GamePlayingScene::~GamePlayingScene()
{
	DeleteGraph(m_handle);
}

void GamePlayingScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void GamePlayingScene::Draw()
{
	(this->*m_drawFunc)();
}

void GamePlayingScene::FadeInUpdate(Input& input)
{
	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &GamePlayingScene::NormalUpdate;
		m_drawFunc = &GamePlayingScene::NormalDraw;
	}
}

void GamePlayingScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_updateFunc = &GamePlayingScene::FadeOutUpdate;
		m_drawFunc = &GamePlayingScene::FadeDraw;
	}
	else if (input.IsTriggered("pause"))
	{
		m_manager.PushScene(std::make_shared<PauseScene>(m_manager));
	}
	m_fps = GetFPS();
}

void GamePlayingScene::FadeOutUpdate(Input& input)
{
	m_frame++;
	if (60 <= m_frame)
	{
		m_manager.ChangeScene(std::make_shared<GameOverScene>(m_manager));
	}
}

void GamePlayingScene::FadeDraw()
{
	DrawString(10, 100, L"GamePlayingScene", 0xffffff);
	DrawGraph(100, 100, m_handle, true);

	int alpha = 255 * (static_cast<float>(m_frame) / 60.0f);
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GamePlayingScene::NormalDraw()
{
	DrawString(10, 100, L"GamePlayingScene", 0xffffff);
	DrawGraph(100, 100, m_handle, true);
	DrawFormatString(10, 10, 0xffffff, L"fps = %2.2f", m_fps);
}
