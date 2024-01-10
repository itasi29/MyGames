#include "GameManager.h"



GameManager::GameManager()
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

FileManager& GameManager::GetFile()
{
	return m_file;
}

StageManager& GameManager::GetStage()
{
	return m_stage;
}

SceneManager& GameManager::GetScene()
{
	return m_scene;
}

SoundSystem& GameManager::GetSound()
{
	return m_sound;
}
