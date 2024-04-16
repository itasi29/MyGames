#include "SceneManager.h"
#include "SceneTitle.h"

SceneManager::SceneManager(FileManager& fileMgr, SoundSystem& sndSys) :
	m_fileMgr(fileMgr),
	m_sndSys(sndSys),
	m_scene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init(Camera& camera)
{
	// 最初のシーンのメモリ確保
	m_scene = std::make_shared<SceneTitle>(*this, m_fileMgr, m_sndSys, camera);
	m_scene->Init();
}

void SceneManager::Update(Input& input)
{
}

void SceneManager::Draw() const
{
	m_scene->Draw();
}

void SceneManager::End()
{
	m_scene->End();
}

void SceneManager::Change()
{
	// 終了処理
	End();
	// シーン変更
	m_scene = m_scene->Change();
	// 初期化処理
	m_scene->Init();
}
