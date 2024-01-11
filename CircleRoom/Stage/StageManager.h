#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Vec2.h"

class Input;
class StageBase;
struct size;

struct StageData
{
	int bestTime = 0;
	bool isClear = false;
};

enum Ability
{
	kNone,
	kDash,
};

/// <summary>
/// �X�e�[�W�̑J�ڂ��R���g���[��
/// </summary>
class StageManager
{
	// ����̓_�������ǂ߂�ǂ���������ȈՎ���
public:
	bool m_clear = false;

public:
	// FIXME:���O�͕ς���
	enum StageDir
	{
		kStageLeft,
		kStageRight,
		kStageUp,
		kStageDown,
		kStageMax,
	};

	StageManager();
	~StageManager();

	// ��ŕύXor����
	void DeleteData();



	void Update(Input& input);
	void Draw();

	/// <summary>
	///  �X�e�[�W�̐؂�ւ�
	/// </summary>
	/// <param name="nextStage">���̃X�e�[�W</param>
	void ChangeStage(std::shared_ptr<StageBase> nextStage);

	/// <summary>
	/// �X�e�[�W�����̂Ƃ��܂ł��܂��������悤
	/// �R�����g�ς����珑������
	/// </summary>
	/// <param name="pos">�������x�N�g��</param>
	/// <param name="handle">�摜�n���h��</param>
	void StartMove(StageManager::StageDir dir, int handle);

	int GetSlideVolumeX(StageDir dir) const;
	int GetSlideVolumeY(StageDir dir) const;

	/// <summary>
	/// �N���A����ۑ�����
	/// </summary>
	/// <param name="path">�p�X</param>
	void Save(const std::string& path);

	/// <summary>
	/// �N���A����ǂݍ���
	/// </summary>
	/// <param name="path">�p�X</param>
	void Load(const std::wstring& path);

	/// <summary>
	/// ���̃X�e�[�W���̃X�e�[�W�f�[�^���Ȃ��ꍇ�쐬����
	/// </summary>
	/// <param name="stgName">�X�e�[�W��</param>
	void CreateData(const std::string& stgName);

	/// <summary>
	/// �N���A���
	/// </summary>
	/// <param name="stgName">�X�e�[�W��</param>
	/// <returns>true:�N���A�ς� / false:���N���A</returns>
	bool IsClearStage(const std::string& stgName);

	/// <summary>
	/// ���łɃN���A�������Ƃ�����{�X��
	/// </summary>
	/// <param name="name">�{�X�̖��O</param>
	/// <returns>true: �N���A���Ă��� / false:�N���A���Ă��Ȃ�</returns>
	bool IsClearBoss(const std::string& name) const;

	/// <summary>
	/// ���łɎE���ꂽ���Ƃ�����G��
	/// </summary>
	/// <param name="name">�G�̖��O</param>
	/// <returns>true: �E���ꂽ���Ƃ����� / false: �E���ꂽ���Ƃ��Ȃ�</returns>
	bool IsKilledEnemy(const std::string& name) const;

	/// <summary>
	/// �X�e�[�W���ɑΉ�����x�X�g�N���A�^�C���������Ă���
	/// </summary>
	/// <param name="stgName">�X�e�[�W��</param>
	/// <returns>�x�X�g�^�C��</returns>
	int GetBestTime(const std::string& stgName) const;

	/// <summary>
	/// �v���C���[���E�����G�̎�ސ���Ԃ�
	/// </summary>
	/// <returns>��ސ�</returns>
	int GetEnemyTypeCount() const;

	/// <summary>
	/// �A�r���e�B��Ԃ�
	/// </summary>
	/// <returns>�A�r���e�B</returns>
	Ability GetAbility() const;

	/// <summary>
	/// �N���A���̕ۑ�
	/// �N���A�ς݂Ƃ���
	/// </summary>
	/// <param name="stgName">�X�e�[�W��</param>
	void SaveClear(const std::string& stgName);

	/// <summary>
	/// �{�X���N���A�������Ƃ�����Ƃ���
	/// </summary>
	/// <param name="name">�{�X�̖��O</param>
	void UpdateClearBoss(const std::string& name);

	/// <summary>
	/// �X�e�[�W���ɑΉ�����x�X�g�^�C���̍X�V
	/// </summary>
	/// <param name="stgName">�X�e�[�W��</param>
	/// <param name="bestTime">�X�V�^�C��</param>
	void UpdateBestTime(const std::string& stgName, int bestTime);

	/// <summary>
	/// �v���C���[���E�����G�����łɎE�������Ƃ����邩�̊m�F
	/// �����A�E�������Ƃ��Ȃ���Ζ��O��ۑ����A��ސ��J�E���g�𑝂₷
	/// </summary>
	/// <param name="name">�G�̖��O</param>
	void UpdateEnemyType(const std::string& name);

	/// <summary>
	/// �A�r���e�B�̕ύX
	/// </summary>
	/// <param name="ability">�A�r���e�B�ԍ�</param>
	void ChangeAbility(Ability ability);

private:
	void UpdateMove();
	void DrawMove();

	void ResetVecX();
	void ResetVecY();

private:
	const size& m_size;

	// �X�e�[�W�̃N���A���Q
	std::unordered_map<std::string, StageData> m_stageSaveData;
	// �E���ꂽ�G�̏��Q
	std::vector<std::string> m_killedEnemyNameTable;
	// �v���C���[�E������ނ̐�
	int m_killedEnemyCount;
	// �N���A�����{�X���Q
	std::vector<std::string> m_clearBossTable;
	// �A�r���e�B
	Ability m_ability;
	// �A�r���e�B�̗L������
	std::unordered_map<Ability, bool> m_abilityActive;

	// �X�e�[�W�̃|�C���^
	std::shared_ptr<StageBase> m_stage;

	// �摜�𓮂����Ă���t���[��
	int m_frame;
	// ��ʂ𓮂����Ă��邩
	bool m_isStageMove;
	// ��ʂ𓮂����x�N�g��
	Vec2 m_vec;
	// �ꏊ
	Vec2 m_pos;
	// ��ʂ̉摜�n���h��
	int m_stageHandle;

};

