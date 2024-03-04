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
	// 文字列
	std::wstring str;
	// フレーム
	int frame;
};

enum class ConditionsType
{
	kTime,
	kKilled,
	kSumTime
};

// ステージの情報構造体
struct StageInfo
{
	// 条件種類
	ConditionsType type = ConditionsType::kTime;
	// 条件情報
	int info = 0;
	std::vector<std::string> infoGroup = {};
};

// 生成情報構造体
struct CreateInfo
{
	// 初期生成数
	int startNum = 0;
	// 初期生成間隔
	int startInterval = 0;
	// 初期ディレイフレーム
	int startDelayFrame = 0;
	// 生成間隔
	int createInterval = 0;
};

// 敵情報構造体
struct EnemyInfo
{
	// 名前
	std::string name = {};
	// 生成数
	int num = 0;
	// 生成情報
	std::vector<CreateInfo> info = {};
};

enum class UpTimeType
{
	kNormal,	// 何もしなくても上がっていく
	kAttack		// 攻撃を与えるときに上がる
};

struct StageData
{
	// 情報更新タイプ
	UpTimeType timeType = UpTimeType::kNormal;
	// 増やすフレーム
	int addTimeFrame = 0;
	// 次ステージ情報
	StageInfo stageInfo = {};
	// 敵情報
	int enemyNum = 0;
	std::vector<EnemyInfo> enemyInfo = {};
	// ボスステージ情報
	bool isBoss = false;
};

class StageBase
{
public:
	StageBase(GameManager& mgr, Input& input, std::shared_ptr<StageMap> map);
	virtual ~StageBase();

	// 初期化
	virtual void Init();
	// 更新
	void Update(Input& input);
	// 描画
	void Draw() const;

	void GenericEnemy(const std::shared_ptr<EnemyBase>& enemy);

	void ChangeStage(const std::string& name) { m_stageName = name; }

	/// <summary>
	/// ステージを変更する
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <returns>true : 入れ替え可能/ fasle : 入れ替え不可能</returns>
	virtual void ChangeStage(Input& input);

	/// <summary>
	/// 特定条件でのタイムの上昇
	/// </summary>
	virtual void UpTime();

	/// <summary>
	/// 最初にステージのクリア条件を確認する
	/// </summary>
	virtual void StartCheck() {};

	/// <summary>
	/// ステージ名を取得する
	/// </summary>
	/// <returns>ステージ名</returns>
	std::string GetStageName() const { return m_stageName; }

	/// <summary>
	/// 敵に殺された/殺されてないの情報の描画
	/// </summary>
	/// <param name="x">描画X座標</param>
	/// <param name="y">描画Y座標</param>
	virtual void DrawEnemyKilledInfo(int x, int y) const;

	/// <summary>
	/// ウェーブする文字の非描画化
	/// </summary>
	void OffDrawWave() { m_isWaveDraw = false; }

protected:
	// 更新関数
	virtual void UpdateSelect(Input& input);
	virtual void UpdatePlaying(Input& input);
	void UpdateAfterBossDeath(Input& input);
	// 描画
	void DrawSelect() const;
	void DrawPlaying() const;
	void DrawAfterBossDeath() const;

	// 条件確認
	void CheckConditionsTime(const std::string& stageName, int timeFrame, int exsitTime, const std::wstring& dir);
	void CheckConditionsSumTime(const std::string& stageName, int timeFrame, int exsitTime, const std::wstring& dir);
	void CheckConditionsKilled(const std::string& stageName, int killedNum, const std::wstring& dir);

	// 条件描画
	void DrawTimeConditions(int y, int handle, int existTime) const;
	void DrawSumTimeConditions(const std::vector<std::string>& names, int y, int handle, int existTime) const;
	void DrawKilledConditions(int y, int handle, int killedNum) const;
	void DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX = false, bool isReverxeY = false) const;

	// 矢印描画
	void DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	void DrawRightArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	void DrawUpArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	void DrawDownArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;

	// 敵タイプ描画
	void DrawKilledEnemy(const std::string& enemyName, int x, int y, int addX, int radius = 16) const;

	// 達成文字追加
	void AddAchivedStr(const std::wstring& dir);

	/*敵の各種生成*/
	void CreateMoveWall();
	void CreateNormal(int& frame, bool isStart = false);
	void CreateLarge(int& frame, bool isStart = false);
	void CreateDash(int& frame, bool isStart = false);
	void CreateEneCreate(int& frame, bool isStart = false);
	void CreateDivision(int& frame, bool isStart = false);

	/*メンバ変数ポインタの変更*/
	void ChangeSelectFunc();
	void ChangePlayingFunc();

	/// <summary>
	/// ステージのクリア確認
	/// </summary>
	/// <param name="timeFrame">タイムフレーム</param>
	virtual void CheckStageConditions(int timeFrame);
	/// <summary>
	/// ステージ条件の描画
	/// </summary>
	/// <param name="drawY">描画Y座標</param>
	/// <returns>背景フレーム描画Y座標</returns>
	virtual int DrawStageConditions(int drawY) const;
	/// <summary>
	/// ステージの矢印描画
	/// </summary>
	virtual void DrawArrow() const;

	/// <summary>
	/// 敵の生成
	/// </summary>
	virtual void CreateEnemy();

	/// <summary>
	/// 強化ボスの生成
	/// </summary>
	virtual void CreateStrongBoss();

	/// <summary>
	/// 特殊描画
	/// </summary>
	virtual void DrawUnique() const {}

	/// <summary>
	/// 時間の更新処理
	/// </summary>
	virtual void UpdateTime();

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
	// ステージ情報読み込み
	void LoadStageInfo();
	void LoadImportantStageInfo(std::vector<std::string>& strConmaBuf, std::string& stageName, bool& isLoadAllEnemys, int& enemyTypeIndex);
	void LoadEnemys(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllEnmeyInfo, int& enemyInfoIndex);

	// チュートリアルでの処理
	bool UpdateTutorial();
	void DrawTutrial();

	// ゲーム開始処理
	void PlayStart();

	// 敵種類ごとの生成
	void CreateEnemyType(const std::string& name, int& frame, bool isStart = false);

	// 方向に合わせた文字列を返す
	std::wstring GetDirName(MapDir dir);

protected:
	// メンバ変数ポインタ
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)() const;
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	// ステージの情報
	std::unordered_map<std::string, StageData> m_stageData;
	// ステージ名
	std::string m_stageName;

	// マネジャーの参照
	GameManager& m_mgr;
	// サウンドシステム
	std::shared_ptr<SoundSystem> m_soundSys;
	// 入力情報
	const Input& m_input;

	// ウィンドウサイズ
	const size& m_size;
	// フィールドサイズ
	float m_fieldSize;

	/*定数*/
	// ステージ変更可能までの待機時間
	const int kWaitChangeFrame = 30;
	// 敵生成の中心地
	const Vec2 m_centerPos;

	// 文字列を一時描画する用の画面ハンドル
	int m_strScreen;
	// 条件拡大用画面ハンドル
	int m_extScreen;

	// 達成の文字の描画用
	std::list<Achived> m_achived;

	// 画像
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
	// キー・ボタンのファイル
	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	// プレイヤー
	std::shared_ptr<Player> m_player;
	/*敵*/
	// 背面描画
	std::list<std::shared_ptr<EnemyBase>> m_backEnemy;
	// 真ん中描画
	std::list<std::shared_ptr<EnemyBase>> m_enemy;
	// 前面描画
	std::list<std::shared_ptr<EnemyBase>> m_frontEnemy;
	// ボス
	std::shared_ptr<BossBase> m_boss;

	// サウンドのフェードフレーム
	int m_soundFrame;
	// 時間フレーム
	int m_timeFrame;
	// 待機フレーム
	int m_waitFrame;
	// ステージ開始時の条件拡大フレーム
	int m_extRateFrame;

	// ベストタイム点滅フラグ
	bool m_isUpdateBestTime;
	// 条件文字の拡大描画フラグ
	bool m_isExtRate;
	// ウエーブの描画フラグ
	bool m_isWaveDraw;

	// 文字ウェーブ用の角度
	float m_waveAngle;

private:
	protected:
	std::shared_ptr<StageMap> m_map;

	// 敵生成数
	int m_enemyNum;
	// 初期生成数
	std::vector<int> m_enemyStarCreateNum;
	// 敵生成フレーム
	std::vector<int> m_enemyCreateFrame;

	// ステージ入った時にクリアしているか
	std::unordered_map<MapDir, bool> m_isClear;
	// 説明画像
	std::vector<std::shared_ptr<FileBase>> m_explanation;
	std::vector<std::shared_ptr<FileBase>> m_emphasisArrow;
	// 説明用インデックス
	int m_explanationIndex;
	// 説明時
	int m_emphasisFrame;
};

