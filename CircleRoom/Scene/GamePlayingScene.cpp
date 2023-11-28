#include <DxLib.h>
#include <cassert>
#include "Input.h"
#include "Application.h"

#include "SceneManager.h"
#include "GamePlayingScene.h"
#include "GameOverScene.h"
#include "PauseScene.h"

#include "Stage/StageManager.h"
#include "Stage/Stage1_1.h"

namespace
{
	// フィールドサイズの倍率
	// フィールドはwindowsizeの縦幅に倍率をかけたものとする
	constexpr float kSizeScale = 0.4f;

	// フェードのフレーム時間
	constexpr int kFadeFrame = 60;
}

GamePlayingScene::GamePlayingScene(SceneManager& manager) :
	Scene(manager),
	m_windowSize(m_manager.GetApp().GetWindowSize()),
	m_fieldSize(m_windowSize.h * kSizeScale),
	m_screenHandle(0),
	m_frame(kFadeFrame)
{
	// メンバ関数ポインタの設定
	m_updateFunc = &GamePlayingScene::UpdateFadeIn;
	m_drawFunc = &GamePlayingScene::DrawFade;

	// ステージの設定
	m_stage = std::make_shared<StageManager>();
	m_stage->ChangeStage(std::make_shared<Stage1_1>(m_stage, m_windowSize, m_fieldSize));
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

void GamePlayingScene::UpdateFadeIn(Input& input)
{
	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &GamePlayingScene::UpdateNormal;
		m_drawFunc = &GamePlayingScene::DrawNormal;
		m_frame = 0;
	}
}

void GamePlayingScene::UpdateFadeOut(Input& input)
{
	m_frame++;
	if (m_frame >= kFadeFrame)
	{
		m_manager.ChangeScene(std::make_shared<GameOverScene>(m_manager));
	}
}

void GamePlayingScene::UpdateNormal(Input& input)
{
	m_stage->Update(input);
}

void GamePlayingScene::UpdateChangeStage(Input& input)
{
}

void GamePlayingScene::DrawFade()
{
	DrawWall();
	m_stage->Draw();

	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GamePlayingScene::DrawNormal()
{
	DrawWall();

	m_stage->Draw();
}

void GamePlayingScene::DrawChangeStage()
{
}

void GamePlayingScene::DrawWall()
{
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

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
}

