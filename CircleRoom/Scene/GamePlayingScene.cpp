#include <DxLib.h>
#include <cassert>
#include "Input.h"
#include "Application.h"
#include "Common/GameManager.h"

#include "GamePlayingScene.h"
#include "GameOverScene.h"
#include "PauseScene.h"

#include "Stage/Stage1_1.h"

namespace
{
	// フィールドサイズの倍率
	// フィールドはwindowsizeの縦幅に倍率をかけたものとする
	constexpr float kSizeScale = 0.4f;

	// フェードのフレーム時間
	constexpr int kFadeFrame = 60;
}

GamePlayingScene::GamePlayingScene(GameManager& mgr) :
	Scene(mgr),
	m_screenHandle(0),
	m_frame(kFadeFrame)
{
	Application& app = Application::GetInstance();
	m_fieldSize = app.GetWindowSize().h * kSizeScale;

	// メンバ関数ポインタの設定
	m_updateFunc = &GamePlayingScene::UpdateFadeIn;
	m_drawFunc = &GamePlayingScene::DrawFade;

	// ステージの設定
	m_mgr.GetStage().ChangeStage(std::make_shared<Stage1_1>(m_mgr, m_fieldSize));

	m_mgr.GetStage().m_clear = false;
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
		m_mgr.GetScene().ChangeScene(std::make_shared<GameOverScene>(m_mgr));
	}
}

void GamePlayingScene::UpdateNormal(Input& input)
{
	m_mgr.GetStage().Update(input);

	// pauseボタンが押されたらポーズ画面を開く
	if (input.IsPress("pause"))
	{
		m_mgr.GetScene().PushScene(std::make_shared<PauseScene>(m_mgr));
	}

	// 簡易実装
	if (m_mgr.GetStage().m_clear)
	{
		m_updateFunc = &GamePlayingScene::UpdateFadeOut;
		m_drawFunc = &GamePlayingScene::DrawFade;
		m_frame = 0;
	}
}

void GamePlayingScene::DrawFade()
{
	m_mgr.GetStage().Draw();

	const auto& m_size = Application::GetInstance().GetWindowSize();
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, m_size.w, m_size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GamePlayingScene::DrawNormal()
{
	m_mgr.GetStage().Draw();
}

