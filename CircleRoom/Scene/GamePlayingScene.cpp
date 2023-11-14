#include <DxLib.h>
#include <cassert>
#include "../Common/Input.h"
#include "../Application.h"

#include "SceneManager.h"
#include "GamePlayingScene.h"
#include "GameOverScene.h"
#include "PauseScene.h"

#include "../Player/Player.h"

namespace
{
	// フェードのフレーム時間
	constexpr int kFadeFrame = 60;
}

GamePlayingScene::GamePlayingScene(SceneManager& manager) :
	Scene(manager)
{
	// フレームの時間
	m_frame = kFadeFrame;
	m_updateFunc = &GamePlayingScene::FadeInUpdate;
	m_drawFunc = &GamePlayingScene::FadeDraw;
	m_player = std::make_shared<Player>(m_manager.GetApp());
}

GamePlayingScene::~GamePlayingScene()
{
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

	m_player->Update(input);
}

void GamePlayingScene::FadeOutUpdate(Input& input)
{
	m_frame++;
	if (m_frame >= kFadeFrame)
	{
		m_manager.ChangeScene(std::make_shared<GameOverScene>(m_manager));
	}
}

void GamePlayingScene::FadeDraw()
{
	DrawString(10, 100, L"GamePlayingScene", 0xffffff);

	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GamePlayingScene::NormalDraw()
{
	DrawString(10, 100, L"GamePlayingScene", 0xffffff);
	DrawFormatString(10, 10, 0xffffff, L"fps = %2.2f", m_fps);

	m_player->Draw();
}
