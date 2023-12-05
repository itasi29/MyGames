#pragma once
#include <vector>
#include <memory>
#include <string>

class StageManager;
class Player;
class EnemyBase;
class Input;
struct Size;
struct StageData;

class StageBase
{
public:
	StageBase(StageManager& mgr, const Size& windowSize, float fieldSize);
	virtual ~StageBase();

	void Update(Input& input);
	void Draw();

	/// <summary>
	/// �X�e�[�W�̃N���A�m�F
	/// </summary>
	virtual void CheckStageConditions() = 0;
	/// <summary>
	/// �X�e�[�W�����̕`��
	/// </summary>
	/// <param name = "isPlaying">true:�v���C��, false:�I��</param>
	virtual void DrawStageConditions(bool isPlaying = false) = 0;
	/// <summary>
	/// �e�X�e�[�W�̏���������
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// �G�̐���
	/// </summary>
	virtual void CreateEnemy() = 0;

	/// <summary>
	/// �X�e�[�W��ύX����
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <returns>true : ����ւ��\/ fasle : ����ւ��s�\</returns>
	virtual void ChangeStage(Input& input) = 0;

	/// <summary>
	/// ���݂̃f�[�^��ۑ�����
	/// </summary>
	virtual void SaveInf() const = 0;

protected:
	// �X�e�[�W�ύX�\�܂ł̑ҋ@����
	const int kWaitChangeFrame = 30;

	StageManager& m_mgr;

	// �N���A�f�[�^�e�[�u��
	std::vector<StageData> m_clearDataTable;

	// Window�T�C�Y
	const Size& m_windowSize;
	// �t�B�[���h�T�C�Y
	float m_fieldSize;

	// �X�e�[�W��
	std::wstring m_stageName;

	// �v���C���[
	std::shared_ptr<Player> m_player;
	// �G
	std::vector<std::shared_ptr<EnemyBase>> m_enemy;

	// �o�ߎ���
	int m_frame;
	
	// �ҋ@����
	int m_waitFrame;

protected:
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)();

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	/// <summary>
	/// �I�𒆂̍X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void UpdateSelect(Input& input);
	/// <summary>
	/// �v���C���̍X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void UpdatePlaying(Input& input);

	/// <summary>
	/// �I�𒆂̕`�揈��
	/// </summary>
	void DrawSelect();
	/// <summary>
	/// �v���C���̕`�揈��
	/// </summary>
	void DrawPlaying();

	/// <summary>
	/// ���ɉ�ʂ��X���C�h���鏈��
	/// </summary>
	/// <param name="nextStage">���̃X�e�[�W��shared_ptr</param>
	void SlideLeft(std::shared_ptr<StageBase> nextStage);
	/// <summary>
	/// �E�ɉ�ʂ��X���C�h���鏈��
	/// </summary>
	/// <param name="nextStage">���̃X�e�[�W��shared_ptr</param>
	void SlideRight(std::shared_ptr<StageBase> nextStage);
	/// <summary>
	/// ��ɉ�ʂ��X���C�h���鏈��
	/// </summary>
	/// <param name="nextStage">���̃X�e�[�W��shared_ptr</param>
	void SlideUp(std::shared_ptr<StageBase> nextStage);
	/// <summary>
	/// ���ɉ�ʂ��X���C�h���鏈��
	/// </summary>
	/// <param name="nextStage">���̃X�e�[�W��shared_ptr</param>
	void SlideDown(std::shared_ptr<StageBase> nextStage);

private:
	/// <summary>
	/// �ǂ̕`��
	/// </summary>
	void DrawWall();

	/// <summary>
	/// �X���C�h�����̑S�̋��ʏ���
	/// </summary>
	/// <param name="now">���݂̉�ʂ�ۑ����邽�߂̉�ʁu�n���h��</param>
	/// <param name="next">���̉�ʂ�ۑ����邽�߂̉�ʃn���h��</param>
	/// <param name="nextStage">���̃X�e�[�W�̃|�C���^</param>
	void SlideStart(int& now, int& next, const std::shared_ptr<StageBase>& nextStage);

	/// <summary>
	/// �i�ޕ�����̃N���A�����N���A���Ă���Ƃ���
	/// </summary>
	/// <param name="dir">�i�ޕ���</param>
	/// <param name="dir">�i�ޕ����̔���</param>
	/// /// <param name="nextStage">���̃X�e�[�W�̃|�C���^</param>
	void ChangeClearData(int dir, int dirInversion, std::shared_ptr<StageBase>& nextStage) const;
};

