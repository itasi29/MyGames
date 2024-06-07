#include "SceneManager.h"
#include <DxLib.h>
#include <cassert>
#include "Common/Game.h"
#ifdef _DEBUG
#include "SceneDebug.h"
#else
#include "Scene/SceneTitle.h"
#endif

namespace
{
	// �f�t�H���g�t�F�[�h�X�s�[�h
	constexpr float kDefaultFadeSpeed = 1.0f / 60.0f;
}

SceneManager::SceneManager() :
	m_scene(nullptr),
	m_isFade(false),
	m_fadeRate(1.0f),
	m_fadeSpeed(kDefaultFadeSpeed),
	m_preSceneScreen(-1),
	m_psH(-1)
{
}

SceneManager::~SceneManager()
{
	DeleteGraph(m_preSceneScreen);
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
	m_scene = std::make_shared<SceneTitle>();
#endif

	assert(m_rt == -1);
	assert(m_preSceneScreen == -1);
	assert(m_psH == -1);
	m_rt = MakeScreen(Game::kWindowWidth, Game::kWindowHeight, true);
	m_preSceneScreen = MakeScreen(Game::kWindowWidth, Game::kWindowHeight, true);
	m_psH = LoadPixelShader(L"");
	assert(m_rt != -1);
	assert(m_preSceneScreen != -1);
	assert(m_psH != -1);

	m_scene->Init();
}

void SceneManager::Final()
{
	m_scene->Final();

	DeleteGraph(m_preSceneScreen);
	DeleteShader(m_psH);
}

void SceneManager::Update()
{
	FadeUpdate();

	m_scene->Update();
}

void SceneManager::Draw() const
{
	SetDrawScreen(m_rt);
	ClearDrawScreen();

	m_scene->Draw();
	FadeDraw();

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, m_rt, true);
}

void SceneManager::Change(float fadeSpeed)
{
	// �O�V�[���̕ۑ�
	m_preScene = m_scene;
	// �V�[���̐؂�ւ�
	m_scene = m_scene->Change();

	// �O�V�[���̉�ʂ�`��
	SetDrawScreen(m_preSceneScreen);
	ClearDrawScreen();
	m_preScene->Draw();
	// �`���ʂ�߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// �t�F�[�h�̏�����
	m_isFade = true;
	m_fadeRate = 0.0f;
	if (fadeSpeed > 0.0f)
	{
		m_fadeSpeed = fadeSpeed;
	}
	else
	{
		m_fadeSpeed = kDefaultFadeSpeed;
	}
}

void SceneManager::FadeUpdate()
{
	// �t�F�[�h���s��Ȃ��ꍇ�͏I��
	if (!m_isFade) return;

	// �t�F�[�h��i�߂�
	m_fadeRate += m_fadeSpeed;

	// �t�F�[�h������������
	if (m_fadeRate > 1.0f)
	{
		// �O�̃V�[���̏I������
		m_preScene->Final();
		// �폜
		m_preScene.reset();
		m_preScene = nullptr;

		// �����������Ƃ�
		m_fadeRate = 1.0f;
		m_isFade = false;
	}
}

void SceneManager::FadeDraw() const
{
	// �t�F�[�h���s��Ȃ��ꍇ�͏I��
	if (!m_isFade) return;

#ifdef _DEBUG
	DrawFormatString(16, 32, 0xff00ff, L"NowFade : rate = %.2f", m_fadeRate);
#endif

	// FIXME: �����ȍ~�ɏ����V�F�[�_�̃R�[�h�͕ʂ̂Ƃ��ŌJ��Ԃ��g����悤�ɂ���
	
}
