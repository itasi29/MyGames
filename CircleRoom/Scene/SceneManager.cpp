#include <DxLib.h>
#include "Application.h"

#include "SceneManager.h"
#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "Scene.h"
#include "BackgroundScene.h"

namespace
{
	constexpr int kShakeSize = 10;
}

SceneManager::SceneManager(bool isDrawBg) :
	m_isShake(false),
	m_shakeFrame(0),
	m_shakeSize(kShakeSize),
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
	m_bg = std::make_shared<BackgroundScene>(file, true, m_isDrawBg);
}

void SceneManager::Update(Input& input)
{
	// 背景の更新
	m_bg->Update();

	// 末尾のみ実行
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

	m_bg->Draw();
	// 先頭から順に描画(最後に積んだものが最後に描画される)
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
	m_bg->Init(isMoveBg);
	if (m_scenes.empty()) {	// リストが空っぽだったら入れ替えるのではなく
		m_scenes.push_back(nextScene);	// 末尾に追加する
	}
	else {
		m_scenes.back() = nextScene;	// すでに一つ以上あったら末尾のものを入れ替える
	}
}

void SceneManager::ChangeSceneWithClear(std::shared_ptr<Scene> nextScene, bool isMoveBg)
{
	m_bg->Init(isMoveBg);
	// 一度すべて消す
	m_scenes.clear();
	// シーンを追加
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
