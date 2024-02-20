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
class Collision;
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
	void Draw() const;

	/// <summary>
	/// �X�e�[�W�̏���������
	/// </summary>
	virtual void Init();

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
	/// �G�̒ǉ�
	/// </summary>
	void GenericEnemy(const std::shared_ptr<EnemyBase>& enemy);

	/// <summary>
	/// �G�ɎE���ꂽ/�E����ĂȂ��̏��̕`��
	/// </summary>
	/// <param name="x">�`��X���W</param>
	/// <param name="y">�`��Y���W</param>
	virtual void DrawEnemyKilledInfo(int x, int y) const = 0;

	/// <summary>
	/// �X�e�[�W�����擾����
	/// </summary>
	/// <returns>�X�e�[�W��</returns>
	std::string GetStageName() const { return m_stageName; }

	/// <summary>
	/// ��������ł̃^�C���̏㏸
	/// </summary>
	virtual void UpTime() {}

	/// <summary>
	/// �E�F�[�u���镶���̔�`�扻
	/// </summary>
	void OffWaveDraw() { m_isWaveDraw = false; }

protected:
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
	/// �{�X�|������̍X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void UpdateAfterBossDeath(Input& input);

	/// <summary>
	/// �I�𒆂̕`�揈��
	/// </summary>
	void DrawSelect() const;
	/// <summary>
	/// �v���C���̕`�揈��
	/// </summary>
	void DrawPlaying() const;
	/// <summary>
	/// �{�X�|������̕`�揈��
	/// </summary>
	void DrawAfterBossDeath() const;

	/// <summary>
	/// �X�e�[�W�̃N���A�m�F
	/// </summary>
	/// <param name="timeFrame">�^�C���t���[��</param>
	virtual void CheckStageConditions(int timeFrame) = 0;
	/// <summary>
	/// �X�e�[�W�����̕`��
	/// </summary>
	/// <param name="drawY">�`��Y���W</param>
	/// <returns>�w�i�t���[���`��Y���W</returns>
	virtual int DrawStageConditions(int drawY) const = 0;
	/// <summary>
	/// �X�e�[�W�̖��`��
	/// </summary>
	virtual void DrawArrow() const = 0;

	/// <summary>
	/// �G�̐���
	/// </summary>
	virtual void CreateEnemy() = 0;

	/// <summary>
	/// �����{�X�̐���
	/// </summary>
	virtual void CreateStrongBoss() {}

	/// <summary>
	/// ����I������
	/// </summary>
	virtual void UniqueEndProcessing() {}
	/// <summary>
	/// ����`��
	/// </summary>
	virtual void DrawUnique() const {}

	/// <summary>
	/// ���Ԃ̍X�V����
	/// </summary>
	virtual void UpdateTime() {}

	/// <summary>
	/// �N���A���Ԃ𒴂��Ă��邩�̊m�F
	/// </summary>
	/// <param name="stageName">�m�F����X�e�[�W��</param>
	/// <param name="timeFrame">�^�C���t���[��</param>
	/// <param name="exsitTime">�N���A����(�b)</param>
	/// <param name="dir">������</param>
	void CheckConditionsTime(const std::string& stageName, int timeFrame, int exsitTime, const std::wstring& dir);
	/// <summary>
	/// �E���ꂽ���������Ă��邩�̊m�F
	/// </summary>
	/// <param name="stageName">�m�F����X�e�[�W��</param>
	/// <param name="killedNum">�E������ސ�</param>
	/// <param name="dir">������</param>
	void CheckConditionsKilled(const std::string& stageName, int killedNum, const std::wstring& dir);
	/// <summary>
	/// ����X�e�[�W�S�Ă̐������Ԃ��N���A���Ԃ𒴂��Ă��邩
	/// </summary>
	/// <param name="stageName">�m�F����X�e�[�W��</param>
	/// <param name="names">�m�F����X�e�[�W�̖��O�Q</param>
	/// <param name="exsitTime">�N���A����(�b)</param>
	/// <param name="dir">������</param>
	void CheckConditionsSumTime(const std::string& stageName, const std::vector<std::string>& names, int timeFrame, int exsitTime, const std::wstring& dir);

	/// <summary>
	/// �N���A�����ɂ�����̕`��
	/// </summary>
	/// <param name="nextStName">���̃X�e�[�W��</param>
	/// <param name="y">�`��Y���W</param>
	/// <param name="angle">���p�x(���̎��v���)</param>
	/// <param name="isReverseX">���]X</param>
	/// <param name="isReverxeY">���]Y/param>
	void DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX = false, bool isReverxeY = false) const;
	/// <summary>
	/// �N���A�����̃^�C����`��
	/// </summary>
	/// <param name="y">�`��Y���W</param>
	/// <param name="handle">�t�H���g�n���h��</param>
	/// <param name="existTime">�N���A����(�b)</param>
	void DrawTimeConditions(int y, int handle, int existTime) const;
	/// <summary>
	/// �N���A�����̓G�ɎE������ސ���`��
	/// </summary>
	/// <param name="y">�`��Y���W</param>
	/// <param name="handle">�t�H���g�n���h��</param>
	/// <param name="killedNum">�|������ސ�</param>
	void DrawKilledConditions(int y, int handle, int killedNum) const;
	/// <summary>
	/// �N���A�����̍��v���Ԃ̂�`��
	/// </summary>
	/// <param name="names">�m�F����X�e�[�W�Q���</param>
	/// <param name="y">�`��Y���W</param>
	/// <param name="handle">�t�H���g�n���h��</param>
	/// <param name="existTime">���v�N���A����(�b)</param>
	void DrawSumTimeConditions(const std::vector<std::string>& names, int y, int handle, int existTime) const;

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
	/// <param name="radius">���a : def = 16</param>
	void DrawKilledEnemy(const std::string& enemyName, int x, int y, int addX, int radius = 16) const;

	void DrawExpansion() const;

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

	/*�����o�ϐ��|�C���^�̕ύX*/
	void ChangeSelectFunc();
	void ChangePlayingFunc();

private:
	/// <summary>
	/// �G�̍X�V
	/// </summary>
	/// <param name="enemys">�G�̔z��</param>
	/// <param name="isDash">�v���C���[���_�b�V�����Ă��邩</param>
	/// <param name="col">�v���C���[�̓����蔻��</param>
	void UpdateEnemy(std::list<std::shared_ptr<EnemyBase>>& enemys, bool isDash, const Collision& col);

	/// <summary>
	/// �{�X�̎��S����
	/// </summary>
	void BossDeath();

	/// <summary>
	/// ���݂̃^�C����`��
	/// </summary>
	/// <param name="x">�`��X���W</param>
	/// <param name="y">�`��Y���W</param>
	/// <param name="handle">�t�H���g�n���h��</param>
	void DrawTime(int x, int y, int handle) const;

	void DrawBestTime() const;

	/// <summary>
	/// �����B���̕`��
	/// </summary>
	/// <param name="y">�`��Y���W</param>
	void DrawConditionsAchived(int y) const;

	/// <summary>
	/// ���̏�Ɍ��̕`��
	/// </summary>
	void DrawArrowLock(int x, int y, bool isBossStage, bool isClear) const;

	/// <summary>
	/// �ǂ̕`��
	/// </summary>
	void DrawWall() const;

	/// <summary>
	/// �I�𓙂̕����E�摜�`��
	/// </summary>
	/// /// <param name="cmd">�R�}���h��</param>
	/// <param name="str">�E�F�[�u�����镶����</param>
	/// <param name="num">������</param>
	void DrawWave(const char* const cmd, const wchar_t* const str[], int num) const;

	/// <summary>
	/// ���̉摜���ǂ�ɂ��邩���߂�
	/// </summary>
	/// <param name="isAlreadyClear">���ɃX�e�[�W���N���A�������Ƃ����邩</param>
	/// <param name="nextStName">���̃X�e�[�W��</param>
	/// <returns>�Ή������摜�n���h��</returns>
	int GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const;

protected:
	// �����o�ϐ��|�C���^
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)() const;
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	// �}�l�W���[�̎Q��
	GameManager& m_mgr;
	// �T�E���h�V�X�e��
	std::shared_ptr<SoundSystem> m_soundSys;
	// ���͏��
	const Input& m_input;

	// �E�B���h�E�T�C�Y
	const size& m_size;
	// �t�B�[���h�T�C�Y
	float m_fieldSize;

	/*�萔*/
	// �X�e�[�W�ύX�\�܂ł̑ҋ@����
	const int kWaitChangeFrame = 30;
	// �G�����̒��S�n
	const Vec2 m_centerPos;

	// ��������ꎞ�`�悷��p�̉�ʃn���h��
	int m_strScreen;
	// �����g��p��ʃn���h��
	int m_extScreen;

	// �摜
	std::shared_ptr<FileBase> m_fieldImg;
	std::shared_ptr<FileBase> m_arrowImg;
	std::shared_ptr<FileBase> m_arrowFlashImg;
	std::shared_ptr<FileBase> m_arrowNoImg;
	std::shared_ptr<FileBase> m_arrowLockImg;
	std::shared_ptr<FileBase> m_arrowConditionsImg;
	std::shared_ptr<FileBase> m_frameImg;
	std::shared_ptr<FileBase> m_backFrameImg;
	std::unordered_map<std::string, std::shared_ptr<FileBase>> m_enemysImg;
	std::shared_ptr<FileBase> m_checkImg;
	// BGM
	std::shared_ptr<FileBase> m_selectBgm;
	std::shared_ptr<FileBase> m_playBgm;
	// SE
	std::shared_ptr<FileBase> m_clearSe;
	// �L�[�E�{�^���̃t�@�C��
	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	// �v���C���[
	std::shared_ptr<Player> m_player;
	/*�G*/
	// �w�ʕ`��
	std::list<std::shared_ptr<EnemyBase>> m_backEnemy;
	// �^�񒆕`��
	std::list<std::shared_ptr<EnemyBase>> m_enemy;
	// �O�ʕ`��
	std::list<std::shared_ptr<EnemyBase>> m_frontEnemy;
	// �{�X
	std::shared_ptr<BossBase> m_boss;

	// �X�e�[�W��
	std::string m_stageName;

	// �B���̕����̕`��p
	std::list<Achived> m_achived;

	// �T�E���h�̃t�F�[�h�t���[��
	int m_soundFrame;
	// ���ԃt���[��
	int m_timeFrame;
	// �ҋ@�t���[��
	int m_waitFrame;
	// �X�e�[�W�J�n���̏����g��t���[��
	int m_extRateFrame;

	// �x�X�g�^�C���_�Ńt���O
	bool m_isUpdateBestTime;
	// ���������̊g��`��t���O
	bool m_isExtRate;
	// �E�G�[�u�̕`��t���O
	bool m_isWaveDraw;

	// �����E�F�[�u�p�̊p�x
	float m_waveAngle;
};

