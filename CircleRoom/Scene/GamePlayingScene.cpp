#include <DxLib.h>
#include <cassert>
#include "../Common/Input.h"
#include "../Application.h"

#include "SceneManager.h"
#include "GamePlayingScene.h"
#include "GameOverScene.h"
#include "PauseScene.h"

#include "../Player/Player.h"
#include "../Enemy/EnemyNormal.h"

namespace
{
	// フェードのフレーム時間
	constexpr int kFadeFrame = 60;

	// 敵生成間隔フレーム
	constexpr int kCreateFrame = 60 * 5;
}

GamePlayingScene::GamePlayingScene(SceneManager& manager) :
	Scene(manager),
	m_windowSize(m_manager.GetApp().GetWindowSize()),
	m_fieldSize(m_windowSize.h / 3.0f)
{
	// フレームの時間
	m_frame = kFadeFrame;
	m_updateFunc = &GamePlayingScene::FadeInUpdate;
	m_drawFunc = &GamePlayingScene::FadeDraw;
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));

	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;
	Vec2 center{centerX, centerY};

	m_enemy[0]->Init(center);
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
		m_frame = 0;
	}
}

void GamePlayingScene::NormalUpdate(Input& input)
{
	//if (input.IsTriggered("OK"))
	//{
	//	m_updateFunc = &GamePlayingScene::FadeOutUpdate;
	//	m_drawFunc = &GamePlayingScene::FadeDraw;
	//  m_frame = 0;
	//}
	//else if (input.IsTriggered("pause"))
	//{
	//	m_manager.PushScene(std::make_shared<PauseScene>(m_manager));
	//}+
	m_frame++;
	m_fps = GetFPS();

	m_player->Update(input);
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();
	}

	if (m_frame > kCreateFrame)
	{
		m_frame = 0;
		m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));
		float centerX = m_windowSize.w * 0.5f;
		float centerY = m_windowSize.h * 0.5f;
		m_enemy.back()->Init(Vec2{centerX, centerY});
	}
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

	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;
	// フィールドの端描画
	// 色は仮
	// 左
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize), 
		0x00ff00);
	// 右
	DrawLine(static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);
	// 上
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		0x00ff00);
	// 下
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	DrawFormatString(10, 116, 0xffff00, L"frame = %d", m_frame);
}
