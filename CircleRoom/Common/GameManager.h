#pragma once

#include "Scene/SceneManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/FileManager.h"

/// <summary>
/// �Q�[���S�̂̃}�l�[�W���[���ꊇ�Ǘ�����
/// </summary>
class GameManager
{
private:
	GameManager();
	// �R�s�[����̋֎~
	GameManager(const GameManager& game) = delete;
	void operator=(const GameManager& game) = delete;

public:
	~GameManager();

	/// <summary>
	/// �Q�[���}�l�[�W���[�N���X�̃C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>�Q�[���}�l�[�W���[�N���X�̃C���X�^���X</returns>
	static GameManager& GetInstance();

	StageManager& GetStage();
	FileManager& GetFile();
	SceneManager& GetScene();

private:
	FileManager m_file;
	StageManager m_stage;
	SceneManager m_scene;
};

