#pragma once
#include <vector>
#include <list>
#include <memory>
#include <string>
#include "Vec2.h"

class GameManager;
class FileBase;
class Player;
class EnemyBase;
class BossBase;
class Input;
struct size;
struct StageData;
enum class StageDir;

class StageBase
{
public:
	StageBase(GameManager& mgr);
	virtual ~StageBase();

	void Update(Input& input);
	void Draw();

	/// <summary>
	/// �e�X�e�[�W�̏���������
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// �ŏ��ɃX�e�[�W�̃N���A�������m�F����
	/// </summary>
	virtual void StartCheck() = 0;

	/// <summary>
	/// �X�e�[�W��ύX����
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <returns>true : ����ւ��\/ fasle : ����ւ��s�\</returns>
	virtual void ChangeStage(Input& input) = 0;

	/// <summary>
	/// �G�{�̂��琶������
	/// </summary>
	void GenericEnemy(const std::shared_ptr<EnemyBase>& enemy);

	/// <summary>
	/// �X�e�[�W�����擾����
	/// </summary>
	/// <returns>�X�e�[�W��</returns>
	std::string GetStageName() const { return m_stageName; }

protected:
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)();

	/// <summary>
	/// �X�e�[�W�̃N���A�m�F
	/// </summary>
	virtual void CheckStageConditions() = 0;
	/// <summary>
	/// �X�e�[�W�����̕`��
	/// </summary>
	/// <param name="drawY">�`�悷��ʒu �����Ȃ����48�������ʒu</param>
	/// <returns>Y���̕`��ʒu</returns>
	virtual int DrawStageConditions(int drawY = 48) = 0;
	/// <summary>
	/// �X�e�[�W�̖��`��
	/// </summary>
	virtual void DrawArrow() const = 0;
	virtual void DrawKilledEnemyType() const = 0;

	/// <summary>
	/// �G�̐���
	/// </summary>
	virtual void CreateEnemy() = 0;

	/// <summary>
	/// �����{�X�̐���
	/// </summary>
	virtual void CreateStrongBoss() {}

	/// <summary>
	/// ���Ԃ̍X�V����
	/// </summary>
	virtual void UpdateTime() {}

	/// <summary>
	/// �I�𒆂̍X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void UpdateSelect(Input& input);
	/// <summary>
	/// �v���C���̍X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void UpdatePlaying(Input& input);

	/// <summary>
	/// �I�𒆂̕`�揈��
	/// </summary>
	void DrawSelect();
	/// <summary>
	/// �v���C���̕`�揈��
	/// </summary>
	void DrawPlaying();

	/// <summary>
	/// �����̕`��
	/// </summary>
	void DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName) const;
	/// <summary>
	/// �E���̕`��
	/// </summary>
	void DrawRightArrow(bool isAlreadyClear, const std::string& nextStName) const;
	/// <summary>
	/// ����̕`��
	/// </summary>
	void DrawUpArrow(bool isAlreadyClear, const std::string& nextStName) const;
	/// <summary>
	/// �����̕`��
	/// </summary>
	void DrawDownArrow(bool isAlreadyClear, const std::string& nextStName) const;

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

	void ChangeSelectFunc();
	void ChangePlayingFunc();

private:
	/// <summary>
	/// �{�X�̎��S����
	/// </summary>
	void BossDeath();

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
	/// /// <param name="nextStage">���̃X�e�[�W�̃|�C���^</param>
	void ChangeClearData(const std::shared_ptr<StageBase>& nextStage) const;

	/// <summary>
	/// �ǂ̕`��
	/// </summary>
	void DrawWall();

protected:
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	GameManager& m_mgr;

	// �������`�悷��p�̉�ʃn���h��
	int m_strHandle;

	// ���̃t���[����`�悷��悤
	std::shared_ptr<FileBase> m_bFrameImg;

	// �E�B���h�E�T�C�Y
	const size& m_size;
	// �t�B�[���h�T�C�Y
	float m_fieldSize;

	// �X�e�[�W�ύX�\�܂ł̑ҋ@����
	const int kWaitChangeFrame = 30;
	const Vec2 m_centerPos;

	// �X�e�[�W��
	std::string m_stageName;

	// �v���C���[
	std::shared_ptr<Player> m_player;
	// �G
	std::list<std::shared_ptr<EnemyBase>> m_enemy;
	// �{�X
	std::shared_ptr<BossBase> m_boss;

	// �o�ߎ���
	int m_frame;
	// ���Ԍo�߂��s�����ǂ���
	bool m_isUpdateTime;

	// �ҋ@����
	int m_waitFrame;
};

