#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Vec2.h"

class Input;
class SceneManager;
class StageBase;
class FileBase;
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
public:
	enum StageDir
	{
		kStageLeft,
		kStageRight,
		kStageUp,
		kStageDown,
		kStageMax,
	};

	StageManager(std::shared_ptr<SceneManager>& mgr);
	~StageManager();

	void Init();

	// ��ŕύXor����
	void InitData();
	void InitPos();

	void Update(Input& input);
	void Draw() const;

	std::string GetStageName() const;

	bool IsClear() const { return m_isClear; }
	void OnClear() { m_isClear = true; }
	void ResetClear() { m_isClear = false; }
	void OffWaveDraw() const;

	/// <summary>
	///  �X�e�[�W�̐؂�ւ�
	/// </summary>
	/// <param name="nextStage">���̃X�e�[�W</param>
	void ChangeStage(std::shared_ptr<StageBase> nextStage);

	/// <summary>
	/// �X�e�[�W�̑����ύX
	/// </summary>
	void ImmediatelyChange();

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
	/// �{�X�X�e�[�W�ɓ��������Ƃ����邩
	/// </summary>
	/// <returns>true: ���� / false: �Ȃ�</returns>
	bool IsBossIn() const { return m_isBossIn; }

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
	bool UpdateBestTime(const std::string& stgName, int bestTime);

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

	/// <summary>
	/// �{�X�X�e�[�W�ɓ��������Ƃ�����Ƃ���
	/// </summary>
	void BossStageIn() { m_isBossIn = true; }

private:

	// �ʏ�̍X�V
	void NormalUpdate(Input& input);
	// �X�e�[�W�ړ��̍X�V
	void StageMoveUpdate(Input& input);
	// �ړ����v���C�̍X�V
	void MoveGamePlaingUpdate(Input& input);

	// �ʏ�̕`��
	void NormalDraw() const;
	// �X�e�[�W�ړ����̕`��
	void StageMoveDraw() const;
	// �ړ����v���C�̕`��
	void MoveGamePlaingDraw() const;

	Vec2 GetPos(const std::string& stage) const;

	void CheckEnd();

private:
	using UpdateFunc_t = void(StageManager::*)(Input&);
	using DrawFunc_t = void(StageManager::*)() const;

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	const size& m_size;

	std::shared_ptr<SceneManager>& m_mgr;

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
	// �{�X�X�e�[�W�ɓ��������Ƃ����邩
	bool m_isBossIn;

	// �X�e�[�W�N���A
	bool m_isClear;

	// �X�e�[�W�̃|�C���^
	std::shared_ptr<StageBase> m_stage;
	// ���̃X�e�[�W�̃|�C���^
	std::shared_ptr<StageBase> m_nextStage;

	// ��ʂ������Ă��邩
	bool m_isMove;

	// ��ʂ𓮂����x�N�g��
	Vec2 m_vec;
	// ���݂̃X�e�[�W�̏ꏊ
	Vec2 m_pos;
	// ���̃X�e�[�W�̏ꏊ
	Vec2 m_targetPos;

	// ��ʕ`��p�n���h��
	int m_drawScreen;
	// �ړ����̉�ʃn���h��
	int m_screen;
	// �L�[�v�p��ʃn���h��
	int m_keepScreen;

	// �_�b�V�������摜�N���X
	std::shared_ptr<FileBase> m_dashImg;
};

