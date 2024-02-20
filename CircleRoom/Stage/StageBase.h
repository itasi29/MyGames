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
	// 文字列
	std::wstring str;
	// フレーム
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
	/// ステージの初期化処理
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 最初にステージのクリア条件を確認する
	/// </summary>
	virtual void StartCheck() = 0;

	/// <summary>
	/// ステージを変更する
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <returns>true : 入れ替え可能/ fasle : 入れ替え不可能</returns>
	virtual void ChangeStage(Input& input) = 0;

	/// <summary>
	/// 敵の追加
	/// </summary>
	void GenericEnemy(const std::shared_ptr<EnemyBase>& enemy);

	/// <summary>
	/// 敵に殺された/殺されてないの情報の描画
	/// </summary>
	/// <param name="x">描画X座標</param>
	/// <param name="y">描画Y座標</param>
	virtual void DrawEnemyKilledInfo(int x, int y) const = 0;

	/// <summary>
	/// ステージ名を取得する
	/// </summary>
	/// <returns>ステージ名</returns>
	std::string GetStageName() const { return m_stageName; }

	/// <summary>
	/// 特定条件でのタイムの上昇
	/// </summary>
	virtual void UpTime() {}

	/// <summary>
	/// ウェーブする文字の非描画化
	/// </summary>
	void OffWaveDraw() { m_isWaveDraw = false; }

protected:
	/// <summary>
	/// 選択中の更新処理
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void UpdateSelect(Input& input);
	/// <summary>
	/// プレイ中の更新処理
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void UpdatePlaying(Input& input);
	/// <summary>
	/// ボス倒した後の更新処理
	/// </summary>
	/// <param name="input">入力情報</param>
	void UpdateAfterBossDeath(Input& input);

	/// <summary>
	/// 選択中の描画処理
	/// </summary>
	void DrawSelect() const;
	/// <summary>
	/// プレイ中の描画処理
	/// </summary>
	void DrawPlaying() const;
	/// <summary>
	/// ボス倒した後の描画処理
	/// </summary>
	void DrawAfterBossDeath() const;

	/// <summary>
	/// ステージのクリア確認
	/// </summary>
	/// <param name="timeFrame">タイムフレーム</param>
	virtual void CheckStageConditions(int timeFrame) = 0;
	/// <summary>
	/// ステージ条件の描画
	/// </summary>
	/// <param name="drawY">描画Y座標</param>
	/// <returns>背景フレーム描画Y座標</returns>
	virtual int DrawStageConditions(int drawY) const = 0;
	/// <summary>
	/// ステージの矢印描画
	/// </summary>
	virtual void DrawArrow() const = 0;

	/// <summary>
	/// 敵の生成
	/// </summary>
	virtual void CreateEnemy() = 0;

	/// <summary>
	/// 強化ボスの生成
	/// </summary>
	virtual void CreateStrongBoss() {}

	/// <summary>
	/// 特殊終了処理
	/// </summary>
	virtual void UniqueEndProcessing() {}
	/// <summary>
	/// 特殊描画
	/// </summary>
	virtual void DrawUnique() const {}

	/// <summary>
	/// 時間の更新処理
	/// </summary>
	virtual void UpdateTime() {}

	/// <summary>
	/// クリア時間を超えているかの確認
	/// </summary>
	/// <param name="stageName">確認するステージ名</param>
	/// <param name="timeFrame">タイムフレーム</param>
	/// <param name="exsitTime">クリア時間(秒)</param>
	/// <param name="dir">方向名</param>
	void CheckConditionsTime(const std::string& stageName, int timeFrame, int exsitTime, const std::wstring& dir);
	/// <summary>
	/// 殺された数が超えているかの確認
	/// </summary>
	/// <param name="stageName">確認するステージ名</param>
	/// <param name="killedNum">殺される種類数</param>
	/// <param name="dir">方向名</param>
	void CheckConditionsKilled(const std::string& stageName, int killedNum, const std::wstring& dir);
	/// <summary>
	/// あるステージ全ての生存時間がクリア時間を超えているか
	/// </summary>
	/// <param name="stageName">確認するステージ名</param>
	/// <param name="names">確認するステージの名前群</param>
	/// <param name="exsitTime">クリア時間(秒)</param>
	/// <param name="dir">方向名</param>
	void CheckConditionsSumTime(const std::string& stageName, const std::vector<std::string>& names, int timeFrame, int exsitTime, const std::wstring& dir);

	/// <summary>
	/// クリア条件にある矢印の描画
	/// </summary>
	/// <param name="nextStName">次のステージ名</param>
	/// <param name="y">描画Y座標</param>
	/// <param name="angle">矢印角度(上基準の時計回り)</param>
	/// <param name="isReverseX">反転X</param>
	/// <param name="isReverxeY">反転Y/param>
	void DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX = false, bool isReverxeY = false) const;
	/// <summary>
	/// クリア条件のタイムを描画
	/// </summary>
	/// <param name="y">描画Y座標</param>
	/// <param name="handle">フォントハンドル</param>
	/// <param name="existTime">クリア時間(秒)</param>
	void DrawTimeConditions(int y, int handle, int existTime) const;
	/// <summary>
	/// クリア条件の敵に殺される種類数を描画
	/// </summary>
	/// <param name="y">描画Y座標</param>
	/// <param name="handle">フォントハンドル</param>
	/// <param name="killedNum">倒される種類数</param>
	void DrawKilledConditions(int y, int handle, int killedNum) const;
	/// <summary>
	/// クリア条件の合計時間のやつ描画
	/// </summary>
	/// <param name="names">確認するステージ群情報</param>
	/// <param name="y">描画Y座標</param>
	/// <param name="handle">フォントハンドル</param>
	/// <param name="existTime">合計クリア時間(秒)</param>
	void DrawSumTimeConditions(const std::vector<std::string>& names, int y, int handle, int existTime) const;

	/// <summary>
	/// 左矢印の描画
	/// </summary>
	void DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	/// <summary>
	/// 右矢印の描画
	/// </summary>
	void DrawRightArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	/// <summary>
	/// 上矢印の描画
	/// </summary>
	void DrawUpArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;
	/// <summary>
	/// 下矢印の描画
	/// </summary>
	void DrawDownArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage = false, bool isClear = false) const;

	/// <summary>
	/// 敵タイプの描画
	/// </summary>
	/// <param name="enemyName">名前</param>
	/// <param name="addX">X座標の追加値</param>
	/// <param name="radius">半径 : def = 16</param>
	void DrawKilledEnemy(const std::string& enemyName, int x, int y, int addX, int radius = 16) const;

	void DrawExpansion() const;

	/// <summary>
	/// クリアした文字の描画群に追加
	/// </summary>
	/// <param name="dir">方向</param>
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

private:
	/// <summary>
	/// 敵の更新
	/// </summary>
	/// <param name="enemys">敵の配列</param>
	/// <param name="isDash">プレイヤーがダッシュしているか</param>
	/// <param name="col">プレイヤーの当たり判定</param>
	void UpdateEnemy(std::list<std::shared_ptr<EnemyBase>>& enemys, bool isDash, const Collision& col);

	/// <summary>
	/// ボスの死亡処理
	/// </summary>
	void BossDeath();

	/// <summary>
	/// 現在のタイムを描画
	/// </summary>
	/// <param name="x">描画X座標</param>
	/// <param name="y">描画Y座標</param>
	/// <param name="handle">フォントハンドル</param>
	void DrawTime(int x, int y, int handle) const;

	void DrawBestTime() const;

	/// <summary>
	/// 条件達成の描画
	/// </summary>
	/// <param name="y">描画Y座標</param>
	void DrawConditionsAchived(int y) const;

	/// <summary>
	/// 矢印の上に鍵の描画
	/// </summary>
	void DrawArrowLock(int x, int y, bool isBossStage, bool isClear) const;

	/// <summary>
	/// 壁の描画
	/// </summary>
	void DrawWall() const;

	/// <summary>
	/// 選択等の文字・画像描画
	/// </summary>
	/// /// <param name="cmd">コマンド名</param>
	/// <param name="str">ウェーブさせる文字列</param>
	/// <param name="num">文字列数</param>
	void DrawWave(const char* const cmd, const wchar_t* const str[], int num) const;

	/// <summary>
	/// 矢印の画像をどれにするか決める
	/// </summary>
	/// <param name="isAlreadyClear">既にステージをクリアしたことがあるか</param>
	/// <param name="nextStName">次のステージ名</param>
	/// <returns>対応した画像ハンドル</returns>
	int GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const;

protected:
	// メンバ変数ポインタ
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)() const;
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

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

	// ステージ名
	std::string m_stageName;

	// 達成の文字の描画用
	std::list<Achived> m_achived;

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
};

