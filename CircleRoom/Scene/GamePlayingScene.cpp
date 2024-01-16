#include <DxLib.h>
#include <cassert>
#include "Input.h"
#include "Application.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/SoundSystem.h"

#include "GamePlayingScene.h"
#include "GameOverScene.h"
#include "OptionScene.h"

#include "FileSystem/FileManager.h"
#include "FileSystem/ImageFile.h"

#include "Stage/Stage1_1.h"
#include "Stage/StageTutorial.h"

namespace
{

	// フェードのフレーム時間
	constexpr int kFadeFrame = 60;
}

GamePlayingScene::GamePlayingScene(GameManager& mgr) :
	Scene(mgr),
	m_screenHandle(0),
	m_frame(kFadeFrame)
{
	// メンバ関数ポインタの設定
	m_updateFunc = &GamePlayingScene::UpdateFadeIn;
	m_drawFunc = &GamePlayingScene::DrawFade;

	// ステージの設定
	// チュートリアルステージを通常ステージに
	if (m_mgr.GetStage()->IsClearStage("StageTutorial"))
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_1>(m_mgr));
	}
	// していなければチュートリアルステージに
	else
	{
		m_mgr.GetStage()->ChangeStage(std::make_shared<StageTutorial>(m_mgr));
	}

	m_mgr.GetStage()->m_clear = false;

	m_bgm = m_mgr.GetFile()->LoadSound(L"provisionalBgm.mp3");
	m_bg = m_mgr.GetFile()->LoadGraphic(L"BG/bg.png");
}

GamePlayingScene::~GamePlayingScene()
{
}

void GamePlayingScene::Update(Input& input)
{
	// うるさいから消す
//	PlaySoundMem(m_bgm->GetHandle(), DX_PLAYTYPE_BACK, false);
	m_mgr.GetSound()->PlayBgm(m_bgm->GetHandle());

	(this->*m_updateFunc)(input);
}

void GamePlayingScene::Draw()
{
	DrawGraph(0, 0, m_bg->GetHandle(), true);

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
		m_mgr.GetScene()->ChangeScene(std::make_shared<GameOverScene>(m_mgr));
	}
}

void GamePlayingScene::UpdateNormal(Input& input)
{
	m_mgr.GetStage()->Update(input);

	// pauseボタンが押されたらポーズ画面を開く
	if (input.IsPress("pause"))
	{
		m_mgr.GetScene()->PushScene(std::make_shared<OptionScene>(m_mgr, input));
	}

	// 簡易実装
	if (m_mgr.GetStage()->m_clear)
	{
		m_updateFunc = &GamePlayingScene::UpdateFadeOut;
		m_drawFunc = &GamePlayingScene::DrawFade;
		m_frame = 0;
	}
}

void GamePlayingScene::DrawFade()
{
	m_mgr.GetStage()->Draw();

	const auto& m_size = Application::GetInstance().GetWindowSize();
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, m_size.w, m_size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GamePlayingScene::DrawNormal()
{
	m_mgr.GetStage()->Draw();
}

