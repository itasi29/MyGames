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

GamePlayingScene::GamePlayingScene(SceneManager& scnMgr, StageManager& stgMgr) :
	Scene(scnMgr, stgMgr),
	m_windowSize(m_scnMgr.GetApp().GetWindowSize()),
	m_fieldSize(m_windowSize.h * kSizeScale),
	m_screenHandle(0),
	m_frame(kFadeFrame)
{
	// メンバ関数ポインタの設定
	m_updateFunc = &GamePlayingScene::UpdateFadeIn;
	m_drawFunc = &GamePlayingScene::DrawFade;

	// ステージの設定
	m_stgMgr.ChangeStage(std::make_shared<Stage1_1>(m_stgMgr, m_windowSize, m_fieldSize));
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
		m_scnMgr.ChangeScene(std::make_shared<GameOverScene>(m_scnMgr, m_stgMgr));
	}
}

void GamePlayingScene::UpdateNormal(Input& input)
{
	m_stgMgr.Update(input);

	// pauseボタンが押されたらポーズ画面を開く
	if (input.IsPress("pause"))
	{
		m_scnMgr.PushScene(std::make_shared<PauseScene>(m_scnMgr, m_stgMgr));
	}
}

void GamePlayingScene::DrawFade()
{
	m_stgMgr.Draw();

	const auto& size = Application::GetInstance().GetWindowSize();
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, size.w, size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GamePlayingScene::DrawNormal()
{
	m_stgMgr.Draw();
}

