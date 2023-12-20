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

StageManager& GameManager::GetStage()
{
	return m_stage;
}

FileManager& GameManager::GetFile()
{
	return m_file;
}

SceneManager& GameManager::GetScene()
{
	return m_scene;
}
