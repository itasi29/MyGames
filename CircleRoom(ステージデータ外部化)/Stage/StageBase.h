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
struct GameData;
enum class StageDir;

struct Achived
{
	// ������
	std::wstring str;
	// �t���[��
	int frame;
};

enum class MapDir
{
	kUp,
	kDown,
	kRight,
	kLeft
};

enum class ConditionsType
{
	kTime,
	kKilled,
	kSumTime
};

// ���X�e�[�W�̏��\����
struct NextStageInfo
{
	// �������
	MapDir dir = MapDir::kUp;
	// ���O
	std::string name = {};
	// �������
	ConditionsType type = ConditionsType::kTime;
	// �������
	int info = 0;
	std::vector<std::string> infoGroup = {};
};

// �������\����
struct CreateInfo
{
	// ����������
	int startNum = 0;
	// ���������Ԋu
	int startInterval = 0;
	// �����f�B���C�t���[��
	int startDelayFrame = 0;
	// �����Ԋu
	int createInterval = 0;
};

// �G���\����
struct EnemyInfo
{
	// ���O
	std::string name = {};
	// ������
	int num = 0;
	// �������
	std::vector<CreateInfo> info = {};
};

struct StageData
{
	// ���X�e�[�W���
	int nextNum = 0;
	std::vector<NextStageInfo> stageInfo = {};
	// �G���
	int enemyNum = 0;
	std::vector<EnemyInfo> enemyInfo = {};
	// �{�X�X�e�[�W���
	bool isBoss = false;
};

class StageBase
{
public:
	StageBase(GameManager& mgr, Input& input);
	virtual ~StageBase();

	// ������
	virtual void Init();
	// �X�V
	void Update(Input& input);
	// �`��
	void Draw() const;

	void GenericEnemy(const std::shared_ptr<EnemyBase>& enemy);

	/// <summary>
	/// �X�e�[�W��ύX����
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <returns>true : ����ւ��\/ fasle : ����ւ��s�\</returns>
	virtual void ChangeStage(Input& input) = 0;

	/// <summary>
	/// ��������ł̃^�C���̏㏸
	/// </summary>
	virtual void UpTime() {}

	/// <summary>
	/// �ŏ��ɃX�e�[�W�̃N���A�������m�F����
	/// </summary>
	virtual void StartCheck() = 0;

	/// <summary>
	/// �X�e�[�W�����擾����
	/// </summary>
	/// <returns>�X�e�[�W��</returns>
	std::string GetStageName() const { return m_stageName; }

	/// <summary>
	/// �G�ɎE���ꂽ/�E����ĂȂ��̏��̕`��
	/// </summary>
	/// <param name="x">�`��X���W</param>
	/// <param name="y">�`��Y���W</param>
	virtual void DrawEnemyKilledInfo(int x, int y) const = 0;

	/// <summary>
	/// �E�F�[�u���镶���̔�`�扻
	/// </summary>
	void OffDrawWave() { m_isWaveDraw = false; }

protected:
	// �X�V�֐�
	virtual void UpdateSelect(Input& input);
	virtual void UpdatePlaying(Input& input);
	void UpdateAfterBossDeath(Input& input);
	// �`��
	void DrawSelect() const;
	void DrawPlaying() const;
	void DrawAfterBossDeath() const;

	// �����m�F
	void CheckConditionsTime(const std::string& stageName, int timeFrame, int exsitTime, const std::wstring& dir);
	void CheckConditionsSumTime(const std::string& stageName, const std::vector<std::string>& names, int timeFrame, int exsitTime, const std::wstring& dir);
	void CheckConditionsKilled(const std::string& stageName, int killedNum, const std::wstring& dir);

	// �����`��
	void DrawTimeConditions(int y, int handle, int existTime) const;
	void DrawSumTimeConditions(const std::vector<std::string>& names, int y, int handle, int existTime) const;
	void DrawKilledConditions(int y, int handle, int killedNum) const;
	void DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX = false, bool isReverxeY = false) const;

	// ���`��
	void DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	void DrawRightArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	void DrawUpArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	void DrawDownArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;

	// �G�^�C�v�`��
	void DrawKilledEnemy(const std::string& enemyName, int x, int y, int addX, int radius = 16) const;

	// �B�������ǉ�
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

private:
	void UpdateEnemy(std::list<std::shared_ptr<EnemyBase>>& enemys, bool isDash, const Collision& col);

	void DeathBoss();

	void DrawTime(int x, int y, int handle) const;
	void DrawBestTime() const;
	void DrawConditionsAchived(int y) const;
	void DrawExpansion() const;
	void DrawWall() const;
	void DrawWave(const char* const cmd, const wchar_t* const str[], int num) const;
	void DrawArrowLock(int x, int y, bool isBossStage, bool isClear) const;

	int GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const;

private:
	// �X�e�[�W���ǂݍ���
	void LoadStageInfo();
	void LoadImportantStageInfo(std::vector<std::string>& strConmaBuf, std::string& stageName, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllNextStages, int& nextStageIndex);
	void LoadEnemys(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllEnmeyInfo, int& enemyInfoIndex);
	void LoadNextStages(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllNextStages, int& nextStageIndex);

protected:
	// �����o�ϐ��|�C���^
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)() const;
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	// �X�e�[�W�̏��
	std::unordered_map<std::string, StageData> m_stageData;

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

