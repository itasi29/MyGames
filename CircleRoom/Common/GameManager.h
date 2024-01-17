#pragma once
#include <memory>

class FileManager;
class StageManager;
class SceneManager;
class SoundSystem;
class FontSystem;

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

	std::shared_ptr<FileManager>& GetFile();
	std::shared_ptr<StageManager>& GetStage();
	std::shared_ptr<SceneManager>& GetScene();
	std::shared_ptr<SoundSystem>& GetSound();
	std::shared_ptr<FontSystem>& GetFont();

private:
	std::shared_ptr<FileManager> m_file;
	std::shared_ptr<StageManager> m_stage;
	std::shared_ptr<SceneManager> m_scene;
	std::shared_ptr<SoundSystem> m_sound;
	std::shared_ptr<FontSystem> m_font;
};

