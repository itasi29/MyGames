#pragma once
#include <vector>
#include <list>
#include <memory>
#include <string>
#include "Vec2.h"

class GameManager;
class FileBase;
class SoundSystem;
class Player;
class EnemyBase;
class BossBase;
class Input;
class BottansFile;
class KeyFile;
struct size;
struct StageData;
enum class StageDir;

struct Achived
{
	// ������
	std::wstring str;
	// �t���[��
	int frame;
};

class StageBase
{
public:
	StageBase(GameManager& mgr, Input& input);
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
	/// �X�e�[�W�̃N���A�m�F
	/// </summary>
	virtual void CheckStageConditions() = 0;
	/// <summary>
	/// �X�e�[�W�����̕`��
	/// </summary>
	/// <param name="drawY">�`�悷��ʒu �����Ȃ����48�������ʒu</param>
	/// <returns>Y���̕`��ʒu</returns>
	virtual int DrawStageConditions(int drawY = 96) = 0;
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
	/// �N���A�����ɂ�����̕`��
	/// </summary>
	/// <param name="nextStName">���X�e�[�W�̂����O</param>
	/// <param name="y">Y���W</param>
	/// <param name="angle">���p�x(���̎��v���)</param>
	/// <param name="isReverseX">���]X</param>
	/// <param name="isReverxeY">���]Y/param>
	void DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX = false, bool isReverxeY = false);
	/// <summary>
	/// �N���A�����̃^�C����`��
	/// </summary>
	/// <param name="y">Y���W</param>
	/// <param name="handle">�t�H���g�n���h��</param>
	/// <param name="existTime">�N���A�^�C��</param>
	void DrawTimeConditions(int y, int handle, int existTime);
	/// <summary>
	/// �N���A�����̓G�^�C�v��`��
	/// </summary>
	/// <param name="y">Y���W</param>
	/// <param name="handle">�t�H���g�n���h��</param>
	/// <param name="killedNum">�|������ސ�</param>
	void DrawKilledConditions(int y, int handle, int killedNum);

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

	void DrawKilledEnemy(const std::string& enemyName, int addX, unsigned int color, int radius = 16) const;

	/// <summary>
	/// �N���A���������̕`��Q�ɒǉ�
	/// </summary>
	/// <param name="dir">����</param>
	void AddAchivedStr(const std::wstring& dir);

	void ChangeSelectFunc();
	void ChangePlayingFunc();

private:
	/// <summary>
	/// �{�X�̎��S����
	/// </summary>
	void BossDeath();

	/// <summary>
	/// ���݂̃^�C����`��
	/// </summary>
	/// <param name="x">X�`��ʒu</param>
	/// <param name="y">Y�`��ʒu</param>
	/// <param name="handle">�t�H���g�n���h��</param>
	void DrawTime(int x, int y, int handle);

	/// <summary>
	/// �����B���̕`��
	/// </summary>
	void DrawConditionsAchived();

	/// <summary>
	/// �ǂ̕`��
	/// </summary>
	void DrawWall();

	/// <summary>
	/// �I�𓙂̉摜�̕`��
	/// </summary>
	void DrawImage();

	int GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const;

protected:
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	GameManager& m_mgr;

	// �摜
	std::shared_ptr<FileBase> m_field;
	std::shared_ptr<FileBase> m_arrow;
	std::shared_ptr<FileBase> m_arrowFlash;
	std::shared_ptr<FileBase> m_arrowNo;
	std::shared_ptr<FileBase> m_arrowConditions;
	std::shared_ptr<FileBase> m_startFrame;

	// ���̃t���[����`�悷��悤
	std::shared_ptr<FileBase> m_bFrameImg;
	// �T�E���h
	std::shared_ptr<SoundSystem> m_sound;
	int m_soundFrame;
	// BGM
	std::shared_ptr<FileBase> m_selectBgm;
	std::shared_ptr<FileBase> m_playBgm;

	// �������`�悷��p�̉�ʃn���h��
	int m_strHandle;

	// �E�B���h�E�T�C�Y
	const size& m_size;
	// �t�B�[���h�T�C�Y
	float m_fieldSize;

	// �X�e�[�W�ύX�\�܂ł̑ҋ@����
	const int kWaitChangeFrame = 30;
	const Vec2 m_centerPos;

	// �X�e�[�W��
	std::string m_stageName;

	// �B���̕����̕`��p
	std::list<Achived> m_achived;

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

	// 
	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	const Input& m_input;
};

