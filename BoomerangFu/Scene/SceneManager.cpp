#include "SceneManager.h"
#include <DxLib.h>
#ifdef _DEBUG
#include "SceneDebug.h"
#else
#include "SceneTitle.h"
#endif
#include "Common/Game.h"
#include "Common/SoundSystem.h"

namespace
{
	constexpr float kFadeFrame = 30.0f;
	constexpr float kFadeSpeed = 1.0f / kFadeFrame;
}

SceneManager::SceneManager() :
	m_scene(nullptr),
	m_updateFunc(&SceneManager::FadeNone),
	m_fadeRate(0.0f),
	m_isFade(false),
	m_bgmH(-1)
{
}

SceneManager::~SceneManager()
{
}

SceneManager& SceneManager::GetInstance()
{
	static SceneManager mgr;
	return mgr;
}

void SceneManager::Init()
{
	// 最初のシーンのメモリ確保
#if _DEBUG
	m_scene = std::make_shared<SceneDebug>();
#else
	m_scene = std::make_shared<SceneTitle>(*this, fileMgr, sndSys, camera);
	m_updateFunc = &SceneManager::FadeIn;
	m_fadeRate = 1.0f;
	m_isFade = true;
#endif

	m_scene->Init();
}

void SceneManager::Update()
{
	(this->*m_updateFunc)();
	m_scene->Update(m_isFade);
}

void SceneManager::Draw() const
{
	m_scene->Draw();

	if (!m_isFade) return;

	// フェード処理
	int alpha = static_cast<int>(255 * m_fadeRate);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Game::kWindowW, Game::kWindowH, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneManager::End()
{
	m_scene->End();
}

void SceneManager::Change(bool isFade)
{
	// 終了処理
	m_scene->End();

	if (isFade)
	{
		// フェード処理初期化
		m_fadeRate = 0.0f;
		m_isFade = true;
		// フェードアウトへ
		m_updateFunc = &SceneManager::FadeOut;
	}
	else
	{
		// シーン変更
		m_scene = m_scene->Change();
		// 初期化処理
		m_scene->Init();
	}
}

void SceneManager::ChangeBgmH(int handle)
{
	m_bgmH = handle;
}

void SceneManager::FadeNone()
{
	// ハンドルが入っていな場合は流さない
	if (m_bgmH > -1)
	{
		auto& snd = SoundSystem::GetInstance();
		snd.PlayBgm(m_bgmH);
	}
}

void SceneManager::FadeIn()
{
	m_fadeRate -= kFadeSpeed;

	// フェードイン完了したら
	if (m_fadeRate < 0.0f)
	{
		// フェード終了
		m_isFade = false;
		// シーンの処理へ
		m_updateFunc = &SceneManager::FadeNone;
	}

	// ハンドルが入っていな場合は流さない
	if (m_bgmH > -1)
	{
		auto& snd = SoundSystem::GetInstance();
		snd.PlayFadeBgm(m_bgmH, 1.0f - m_fadeRate);
	}
}

void SceneManager::FadeOut()
{
	m_fadeRate += kFadeSpeed;

	// フェードアウト完了したら
	if (m_fadeRate > 1.0f)
	{
		// シーン変更
		m_scene = m_scene->Change();
		// 初期化処理
		m_scene->Init();
		// フェード割合補正
		m_fadeRate = 1.0f;
		// フェードインへ
		m_updateFunc = &SceneManager::FadeIn;
	}

	// ハンドルが入っていな場合は流さない
	if (m_bgmH > -1)
	{
		auto& snd = SoundSystem::GetInstance();
		snd.PlayFadeBgm(m_bgmH, 1.0f - m_fadeRate);
	}
}

