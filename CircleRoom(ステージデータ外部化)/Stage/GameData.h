#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

class FileBase;

struct SaveData
{
	int bestTime = 0;
	bool isClear = false;
};

enum class Ability
{
	kNone,
	kDash,
};

/// <summary>
/// �X�e�[�W�f�[�^�������N���X
/// </summary>
class GameData
{
public:
	GameData();
	~GameData();

	// ������
	void Init();
	void InitData();

	// �擾�n
	int GetBestTime(const std::string& name) const;
	int GetEnemyTypeCount() const { return m_killedEnemyTypeNum; }
	Ability GetAbility() const { return m_ability; }

	// ���f�n
	bool IsClearStage(const std::string& name);
	bool IsClearBoss(const std::string& name) const;
	bool IsKilledEnemy(const std::string& name) const;
	bool IsInBoss() const { return m_isInBoss; };

	// �f�[�^�쐬
	void CreateData(const std::string& name);

	// �ۑ��n
	void SaveClearStage(const std::string& name);
	void SaveClearBoss(const std::string& name);
	void SaveKilledEnemyType(const std::string& name);
	bool SaveBestTime(const std::string& name, int frame);
	void SaveInBossStage() { m_isInBoss = true; }

	// �ύX�n
	void ChangeAbility(Ability ability);

	// ��񃍁[�h�E�Z�[�u
	void Save();
	void Load();

private:
	// �Z�[�u�f�[�^���Q
	std::unordered_map<std::string, SaveData> m_saveData;
	// �N���A�����{�X�̏��Q
	std::vector<std::string> m_clearBossData;
	// �E���ꂽ���Ƃ̂���G�̏��Q
	std::vector<std::string> m_killedEnemyNameData;
	// �v���C���[���E�������Ƃ�����G�̎�ސ�
	int m_killedEnemyTypeNum;
	// �A�r���e�B
	Ability m_ability;
	// �A�r���e�B�L�����f
	std::unordered_map<Ability, bool> m_isActiveAbilitys;

	// �{�X�X�e�[�W���ꔻ��
	bool m_isInBoss;

	// �_�b�V�������摜
	std::shared_ptr<FileBase> m_explanationDash;
};

