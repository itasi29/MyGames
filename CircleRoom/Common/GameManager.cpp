#include "GameManager.h"

#include "Scene/SceneManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"

GameManager::GameManager() :
	m_file(std::make_shared<FileManager>()),
	m_stage(std::make_shared<StageManager>()),
	m_scene(std::make_shared<SceneManager>()),
	m_sound(std::make_shared<SoundSystem>())
{
}

GameManager::~GameManager()
{
}

GameManager& GameManager::GetInstance()
{
	static GameManager instance;
	return instance;
}

std::shared_ptr<FileManager>& GameManager::GetFile()
{
	return m_file;
}

std::shared_ptr<StageManager>& GameManager::GetStage()
{
	return m_stage;
}

std::shared_ptr<SceneManager>& GameManager::GetScene()
{
	return m_scene;
}

std::shared_ptr<SoundSystem>& GameManager::GetSound()
{
	return m_sound;
}
