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
	// �ŏ��̃V�[���̃������m��
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

	// �t�F�[�h����
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
	// �I������
	m_scene->End();

	if (isFade)
	{
		// �t�F�[�h����������
		m_fadeRate = 0.0f;
		m_isFade = true;
		// �t�F�[�h�A�E�g��
		m_updateFunc = &SceneManager::FadeOut;
	}
	else
	{
		// �V�[���ύX
		m_scene = m_scene->Change();
		// ����������
		m_scene->Init();
	}
}

void SceneManager::ChangeBgmH(int handle)
{
	m_bgmH = handle;
}

void SceneManager::FadeNone()
{
	// �n���h���������Ă��ȏꍇ�͗����Ȃ�
	if (m_bgmH > -1)
	{
		auto& snd = SoundSystem::GetInstance();
		snd.PlayBgm(m_bgmH);
	}
}

void SceneManager::FadeIn()
{
	m_fadeRate -= kFadeSpeed;

	// �t�F�[�h�C������������
	if (m_fadeRate < 0.0f)
	{
		// �t�F�[�h�I��
		m_isFade = false;
		// �V�[���̏�����
		m_updateFunc = &SceneManager::FadeNone;
	}

	// �n���h���������Ă��ȏꍇ�͗����Ȃ�
	if (m_bgmH > -1)
	{
		auto& snd = SoundSystem::GetInstance();
		snd.PlayFadeBgm(m_bgmH, 1.0f - m_fadeRate);
	}
}

void SceneManager::FadeOut()
{
	m_fadeRate += kFadeSpeed;

	// �t�F�[�h�A�E�g����������
	if (m_fadeRate > 1.0f)
	{
		// �V�[���ύX
		m_scene = m_scene->Change();
		// ����������
		m_scene->Init();
		// �t�F�[�h�����␳
		m_fadeRate = 1.0f;
		// �t�F�[�h�C����
		m_updateFunc = &SceneManager::FadeIn;
	}

	// �n���h���������Ă��ȏꍇ�͗����Ȃ�
	if (m_bgmH > -1)
	{
		auto& snd = SoundSystem::GetInstance();
		snd.PlayFadeBgm(m_bgmH, 1.0f - m_fadeRate);
	}
}

