#include <DxLib.h>
#include "Application.h"

#include "SceneManager.h"
#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "Scene.h"

namespace
{
	constexpr int kShakeSize = 10;
}

SceneManager::SceneManager(bool isDrawBg) :
	m_isShake(false),
	m_shakeFrame(0),
	m_shakeSize(kShakeSize),
	m_bgFrame(0),
	m_isMoveBg(true),
	m_isDrawBg(isDrawBg)
{
	const size size = Application::GetInstance().GetWindowSize();
	m_shakeHandle = MakeScreen(size.w, size.h);
}

SceneManager::~SceneManager()
{
	DeleteGraph(m_shakeHandle);
}

void SceneManager::Init()
{
	if (m_bg) return;

	auto& file = GameManager::GetInstance().GetFile();
	m_bg = file->LoadGraphic(L"BG/bg.png", true);
}

void SceneManager::Update(Input& input)
{
	m_bgFrame--;
	// �����̂ݎ��s
	m_scenes.back()->Update(input);

	if (m_isShake)
	{
		m_shakeFrame--;

		if (m_shakeFrame < 0)
		{
			m_isShake = false;
		}
	}
}

void SceneManager::Draw()
{
	if (m_isShake)
	{
		SetDrawScreen(m_shakeHandle);
		ClearDrawScreen();
	}

	DrawBg();
	// �擪���珇�ɕ`��(�Ō�ɐς񂾂��̂��Ō�ɕ`�悳���)
	for (auto& scene : m_scenes)
	{
		scene->Draw();
	}

	if (m_isShake)
	{
		SetDrawScreen(DX_SCREEN_BACK);

		int x = GetRand(m_shakeSize) - static_cast<int>(m_shakeSize * 0.5f);
		int y = GetRand(m_shakeSize) - static_cast<int>(m_shakeSize * 0.5f);
		DrawGraph(x, y, m_shakeHandle, true);
	}
}

void SceneManager::ChangeScene(std::shared_ptr<Scene>nextScene, bool isMoveBg)
{
	m_isMoveBg = isMoveBg;
	m_bgFrame = 0;
	if (m_scenes.empty()) {	// ���X�g������ۂ����������ւ���̂ł͂Ȃ�
		m_scenes.push_back(nextScene);	// �����ɒǉ�����
	}
	else {
		m_scenes.back() = nextScene;	// ���łɈ�ȏ゠�����疖���̂��̂����ւ���
	}
}

void SceneManager::ChangeSceneWithClear(std::shared_ptr<Scene> nextScene, bool isMoveBg)
{
	m_isMoveBg = isMoveBg;
	m_bgFrame = 0;
	// ��x���ׂď���
	m_scenes.clear();
	// �V�[����ǉ�
	m_scenes.push_back(nextScene);
}

void SceneManager::PushScene(std::shared_ptr<Scene> scene)
{
	m_scenes.push_back(scene);
}

void SceneManager::PopScene()
{
	m_scenes.pop_back();
}

void SceneManager::ShakeScreen(int frame)
{
	ShakeScreen(frame, kShakeSize);
}

void SceneManager::ShakeScreen(int frame, int size = kShakeSize)
{
	m_shakeFrame = frame;
	m_shakeSize = size;
	m_isShake = true;
}

std::shared_ptr<Scene> SceneManager::GetTopScene()
{
	return m_scenes.back();
}

void SceneManager::DrawBg()
{
	if (!m_isDrawBg) return;

	if (!m_isMoveBg)
	{
		DrawGraph(0, 0, m_bg->GetHandle(), false);
		return;
	}
	else
	{
		const auto& size = Application::GetInstance().GetWindowSize();

		int posX = m_bgFrame % size.w;
		DrawGraph(posX, 0, m_bg->GetHandle(), false);
		posX += size.w;
		DrawGraph(posX, 0, m_bg->GetHandle(), false);
	}
}
