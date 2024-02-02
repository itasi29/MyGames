#pragma once
#include <vector>
#include <list>
#include <unordered_map>
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
	
	// FIXME:�Ƃ肠���������������������疼�O�͌�ŕύX������
	/// <summary>
	/// �{�X�|������̍X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void UpdateBossDeath(Input& input);

	/// <summary>
	/// �I�𒆂̕`�揈��
	/// </summary>
	void DrawSelect();
	/// <summary>
	/// �v���C���̕`�揈��
	/// </summary>
	void DrawPlaying();

	// FIXME:�Ƃ肠���������������������疼�O�͌�ŕύX������
	/// <summary>
	/// �{�X�|������̕`�揈��
	/// </summary>
	void DrawBossDeath();

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
	/// �N���A���Ԃ𒴂��Ă��邩�̊m�F
	/// </summary>
	/// <param name="stageName">�m�F����X�e�[�W�̖��O</param>
	/// <param name="exsitTime">�N���A����(�b)</param>
	/// <param name="dir">������</param>
	void CheckConditionsTime(const std::string& stageName, int exsitTime, const std::wstring& dir);
	/// <summary>
	/// �E���ꂽ���������Ă��邩�̊m�F
	/// </summary>
	/// <param name="stageName">�m�F����X�e�[�W�̖��O</param>
	/// <param name="killedNum">�E������ސ�</param>
	/// <param name="dir">������</param>
	void CheckConditionsKilled(const std::string& stageName, int killedNum, const std::wstring& dir);
	/// <summary>
	/// ����X�e�[�W�S�Ă̐������Ԃ��N���A���Ԃ𒴂��Ă��邩
	/// </summary>
	/// <param name="stageName">�N���A�ɂ���X�e�[�W�̖��O</param>
	/// <param name="names">�m�F����X�e�[�W�Q���</param>
	/// <param name="exsitTime">�N���A����(�b)</param>
	/// <param name="dir">������</param>
	void CheckConditionsSumTime(const std::string& stageName, const std::vector<std::string>& names, int exsitTime, const std::wstring& dir);

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
	/// �N���A�����̍��v���Ԃ̂�`��
	/// </summary>
	/// <param name="names">�m�F����X�e�[�W�Q���</param>
	/// <param name="y">Y���W</param>
	/// <param name="handle">�t�H���g�n���h��</param>
	/// <param name="existTime">���v�N���A����</param>
	void DrawSumTimeConditions(const std::vector<std::string>& names, int y, int handle, int existTime);

	/// <summary>
	/// �����̕`��
	/// </summary>
	void DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	/// <summary>
	/// �E���̕`��
	/// </summary>
	void DrawRightArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	/// <summary>
	/// ����̕`��
	/// </summary>
	void DrawUpArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	/// <summary>
	/// �����̕`��
	/// </summary>
	void DrawDownArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;

	/// <summary>
	/// �G�^�C�v�̕`��
	/// </summary>
	/// <param name="enemyName">���O</param>
	/// <param name="addX">X���W�̒ǉ��l</param>
	/// <param name="color">�F</param>
	/// <param name="radius">���a : def = 16</param>
	void DrawKilledEnemy(const std::string& enemyName, int addX, unsigned int color, int radius = 16) const;

	/// <summary>
	/// �N���A���������̕`��Q�ɒǉ�
	/// </summary>
	/// <param name="dir">����</param>
	void AddAchivedStr(const std::wstring& dir);

	/*�G�̊e�퐶��*/
	void CreateMoveWall();
	void CreateNormal(int& frame, bool isStart = false);
	void CreateLarge(int& frame, bool isStart = false);
	void CreateDash(int& frame, bool isStart = false);
	void CreateEneCreate(int& frame, bool isStart = false);
	void CreateDivision(int& frame, bool isStart = false);

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

	void DrawBestTime();

	/// <summary>
	/// �����B���̕`��
	/// </summary>
	/// <param name="y">Y�`��ʒu</param>
	void DrawConditionsAchived(int y);

	/// <summary>
	/// ���̏�Ɍ��̕`��
	/// </summary>
	void DrawArrowLock(int x, int y, bool isBossStage, bool isClear) const;

	/// <summary>
	/// �ǂ̕`��
	/// </summary>
	void DrawWall();

	/// <summary>
	/// �I�𓙂̕����E�摜�`��
	/// </summary>
	/// /// <param name="cmd">�R�}���h��</param>
	/// <param name="str">�E�F�[�u�����镶����</param>
	/// <param name="num">������</param>
	void DrawWave(const char* const cmd, const wchar_t* const str[], int num);

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
	std::shared_ptr<FileBase> m_arrowLock;
	std::shared_ptr<FileBase> m_arrowConditions;
	std::shared_ptr<FileBase> m_startFrame;
	std::unordered_map<std::string, std::shared_ptr<FileBase>> m_enemysImg;
	std::shared_ptr<FileBase> m_check;

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

	// �����E�F�[�u�p�̊p�x
	float m_waveAngle;
	// �E�G�[�u�̂�`�悷�邩
	bool m_isWaveDraw;

	// �x�X�g�^�C���_��
	bool m_isUpdateBestTime;


	// 
	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	const Input& m_input;
};

