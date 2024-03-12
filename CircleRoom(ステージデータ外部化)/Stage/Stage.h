#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <string>
#include "Vec2.h"

class GameManager;
class SoundSystem;
class StageMap;
class FileBase;
class BottansFile;
class KeyFile;
class Collision;
class Input;
class Player;
class EnemyBase;
class BossBase;
struct size;
enum class StageDir;
enum class MapDir;

struct Achived
{
	// ������
	std::wstring str;
	// �t���[��
	int frame;
};

enum class ConditionsType
{
	kTime,
	kKilled,
	kSumTime
};

// �X�e�[�W�̏��\����
struct StageInfo
{
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

enum class UpTimeType
{
	kNormal,	// �������Ȃ��Ă��オ���Ă���
	kAttack		// �U����^����Ƃ��ɏオ��
};

struct StageData
{
	// ���X�V�^�C�v
	UpTimeType timeType = UpTimeType::kNormal;
	// ���₷�t���[��
	int addTimeFrame = 0;
	// ���X�e�[�W���
	StageInfo stageInfo = {};
	// �G���
	int enemyNum = 0;
	std::vector<EnemyInfo> enemyInfo = {};
	// �{�X�X�e�[�W���
	bool isBoss = false;
};

class Stage
{
public:
	Stage(GameManager& mgr, Input& input, std::shared_ptr<StageMap> map);
	~Stage();

	// ������
	void Init();
	// �X�V
	void Update(Input& input);
	// �`��
	void Draw() const;

	/// <summary>
	/// �G�̒ǉ�
	/// </summary>
	/// <param name="enemy">�G�̃|�C���^</param>
	void GenericEnemy(const std::shared_ptr<EnemyBase>& enemy);

	/// <summary>
	/// �^�C���̏㏸
	/// </summary>
	void UpTime();

	/// <summary>
	/// �G�ɎE���ꂽ/�E����ĂȂ��̏��̕`��
	/// </summary>
	/// <param name="x">�`��X���W</param>
	/// <param name="y">�`��Y���W</param>
	void DrawEnemyKilledInfo(int x, int y) const;

	/// <summary>
	/// �X�e�[�W��ύX����
	/// �ڑ������ŏ�����B�����Ă�����ړ�����
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <returns>true : ����ւ��\/ fasle : ����ւ��s�\</returns>
	void ChangeStage(Input& input);

	/// <summary>
	/// �X�e�[�W�ύX����
	/// �w�肵���X�e�[�W�Ƀf�[�^��ύX����
	/// </summary>
	/// <param name="name">�X�e�[�W��</param>
	void ChangeStageData(const std::string& name);

	/// <summary>
	/// �X�e�[�W�����擾����
	/// </summary>
	/// <returns>�X�e�[�W��</returns>
	std::string GetStageName() const { return m_stageName; }

	/// <summary>
	/// �E�F�[�u���镶���̔�`�扻
	/// </summary>
	void OffDrawWave() { m_isWaveDraw = false; }

private:
	// �X�V�֐�
	void UpdateSelect(Input& input);
	void UpdatePlaying(Input& input);
	void UpdateAfterBossDeath(Input& input);
	// �`��
	void DrawSelect() const;
	void DrawPlaying() const;
	void DrawAfterBossDeath() const;

	// �Q�[���J�n����
	void PlayStart();

	// �ׂ��ȍX�V����
	bool UpdateTutorial();
	void UpdateEnemy(std::list<std::shared_ptr<EnemyBase>>& enemys, bool isDash, const Collision& col);
	void UpdateTime();
	// �ׂ��ȕ`�揈��
	void DrawTutrial();
	void DrawWall() const;

	// �����֌W�`��
	int DrawStageConditions(int drawY) const;
	void DrawTimeConditions(int y, int handle, int existTime) const;
	void DrawSumTimeConditions(const std::vector<std::string>& names, int y, int handle, int existTime) const;
	void DrawKilledConditions(int y, int handle, int killedNum) const;
	void DrawConditionsAchived(int y) const;
	void DrawExpansion() const;

	// ���Ԋ֌W�`��
	void DrawTime(int x, int y, int handle) const;
	void DrawBestTime() const;

	// ���`��
	void DrawArrow() const;

	// �G�^�C�v�`��
	void DrawKilledEnemy(const std::string& enemyName, int x, int y, int addX, int radius = 16) const;

	// �h�炬�����`��
	void DrawWave(const char* const cmd, const wchar_t* const str[], int num) const;

	/// <summary>
	/// �G�̐���
	/// </summary>
	void CreateEnemy();
	// �G��ނ��Ƃ̐���
	void CreateEnemyType(const std::string& name, int& frame, bool isStart = false);
	// �����{�X�̐���
	void CreateStrongBoss();
	// �{�X���S����
	void DeathBoss();

	// �����m�F
	void CheckStageConditions(int timeFrame);
	void CheckConditionsTime(const std::string& stageName, int timeFrame, int exsitTime, const std::wstring& dir);
	void CheckConditionsSumTime(const std::string& stageName, int timeFrame, int exsitTime, const std::wstring& dir);
	void CheckConditionsKilled(const std::string& stageName, int killedNum, const std::wstring& dir);

	// �B�������ǉ�
	void AddAchivedStr(const std::wstring& dir);

	// �����ɍ��킹���������Ԃ�
	std::wstring GetDirName(MapDir dir);
	int GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const;
	
	// �X�e�[�W���ǂݍ���
	void LoadStageInfo();
	void LoadImportantStageInfo(std::vector<std::string>& strConmaBuf, std::string& stageName, bool& isLoadAllEnemys, int& enemyTypeIndex);
	void LoadEnemys(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllEnmeyInfo, int& enemyInfoIndex);

protected:
	// �����o�ϐ��|�C���^
	using UpdateFunc_t = void (Stage::*)(Input&);
	using DrawFunc_t = void (Stage::*)() const;
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	// �X�e�[�W�̏��
	std::unordered_map<std::string, StageData> m_stageData;
	// �X�e�[�W��
	std::string m_stageName;

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

	// �}�b�v�f�[�^
	std::shared_ptr<StageMap> m_map;
	// �X�e�[�W���������ɃN���A���Ă��邩
	std::unordered_map<MapDir, bool> m_isClear;

	/*�萔*/
	// �X�e�[�W�ύX�\�܂ł̑ҋ@����
	const int kWaitChangeFrame = 30;
	// �G�����̒��S�n
	const Vec2 m_centerPos;

	// ��������ꎞ�`�悷��p�̉�ʃn���h��
	int m_strScreen;
	// �����g��p��ʃn���h��
	int m_extScreen;

	// �B���̕����̕`��p
	std::list<Achived> m_achived;

	// �摜
	std::vector<std::shared_ptr<FileBase>> m_explanation;
	std::vector<std::shared_ptr<FileBase>> m_emphasisArrow;
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

	// �����p�C���f�b�N�X
	int m_explanationIndex;
	// ������
	int m_emphasisFrame;

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
	// ������
	int m_enemyNum;
	// ����������
	std::vector<int> m_enemyStarCreateNum;
	// �����t���[��
	std::vector<int> m_enemyCreateFrame;

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
