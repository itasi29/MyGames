#pragma once
#include <memory>

class FileManager;
class SceneManager;
class StageManager;
class SoundSystem;
class FontSystem;

struct VolumeData
{
	// Bgm�{�����[��
	int bgmVol;
	// Se�{�����[��
	int seVol;
};

struct AccountData
{
	// �v���C����
	int playTime;
	// ���S��
	int deathCount;
	// �_�b�V���g�p��
	int dashCount;

	VolumeData volume;
};

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

	void Init();
	void DeleteData();

	/// <summary>
	/// �Q�[���}�l�[�W���[�N���X�̃C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>�Q�[���}�l�[�W���[�N���X�̃C���X�^���X</returns>
	static GameManager& GetInstance();

	std::shared_ptr<FileManager>& GetFile();
	std::shared_ptr<SceneManager>& GetScene();
	std::shared_ptr<StageManager>& GetStage();
	std::shared_ptr<SoundSystem>& GetSound();
	std::shared_ptr<FontSystem>& GetFont();

	VolumeData GetVolume() const { return m_data.volume; }
	AccountData GetData() const;

	void UpdateVolume(const VolumeData& data) { m_data.volume = data; }
	void UpdatePlaytime() { m_data.playTime++; }
	void UpdateDeathCcount() { m_data.deathCount++; }
	void UpdateDashCount() { m_data.dashCount++; }

private:
	void Save();
	void Load();

private:
	std::shared_ptr<FileManager> m_file;
	std::shared_ptr<SceneManager> m_scene;
	std::shared_ptr<StageManager> m_stage;
	std::shared_ptr<SoundSystem> m_sound;
	std::shared_ptr<FontSystem> m_font;

	AccountData m_data;
};

