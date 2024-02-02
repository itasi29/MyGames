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
	void Draw();

	/// <summary>
	/// 各ステージの初期化処理
	/// </summary>
	virtual void Init() = 0;

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
	/// 敵本体から生成する
	/// </summary>
	void GenericEnemy(const std::shared_ptr<EnemyBase>& enemy);

	/// <summary>
	/// ステージ名を取得する
	/// </summary>
	/// <returns>ステージ名</returns>
	std::string GetStageName() const { return m_stageName; }

protected:
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)();

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
	
	// FIXME:とりあえず仮実装させたいから名前は後で変更させる
	/// <summary>
	/// ボス倒した後の更新処理
	/// </summary>
	/// <param name="input">入力情報</param>
	void UpdateBossDeath(Input& input);

	/// <summary>
	/// 選択中の描画処理
	/// </summary>
	void DrawSelect();
	/// <summary>
	/// プレイ中の描画処理
	/// </summary>
	void DrawPlaying();

	// FIXME:とりあえず仮実装させたいから名前は後で変更させる
	/// <summary>
	/// ボス倒した後の描画処理
	/// </summary>
	void DrawBossDeath();

	/// <summary>
	/// ステージのクリア確認
	/// </summary>
	virtual void CheckStageConditions() = 0;
	/// <summary>
	/// ステージ条件の描画
	/// </summary>
	/// <param name="drawY">描画する位置 書かなければ48が初期位置</param>
	/// <returns>Y軸の描画位置</returns>
	virtual int DrawStageConditions(int drawY = 96) = 0;
	/// <summary>
	/// ステージの矢印描画
	/// </summary>
	virtual void DrawArrow() const = 0;
	virtual void DrawKilledEnemyType() const = 0;

	/// <summary>
	/// 敵の生成
	/// </summary>
	virtual void CreateEnemy() = 0;

	/// <summary>
	/// 強化ボスの生成
	/// </summary>
	virtual void CreateStrongBoss() {}

	/// <summary>
	/// 時間の更新処理
	/// </summary>
	virtual void UpdateTime() {}

	/// <summary>
	/// クリア時間を超えているかの確認
	/// </summary>
	/// <param name="stageName">確認するステージの名前</param>
	/// <param name="exsitTime">クリア時間(秒)</param>
	/// <param name="dir">方向名</param>
	void CheckConditionsTime(const std::string& stageName, int exsitTime, const std::wstring& dir);
	/// <summary>
	/// 殺された数が超えているかの確認
	/// </summary>
	/// <param name="stageName">確認するステージの名前</param>
	/// <param name="killedNum">殺される種類数</param>
	/// <param name="dir">方向名</param>
	void CheckConditionsKilled(const std::string& stageName, int killedNum, const std::wstring& dir);
	/// <summary>
	/// あるステージ全ての生存時間がクリア時間を超えているか
	/// </summary>
	/// <param name="stageName">クリアにするステージの名前</param>
	/// <param name="names">確認するステージ群情報</param>
	/// <param name="exsitTime">クリア時間(秒)</param>
	/// <param name="dir">方向名</param>
	void CheckConditionsSumTime(const std::string& stageName, const std::vector<std::string>& names, int exsitTime, const std::wstring& dir);

	/// <summary>
	/// クリア条件にある矢印の描画
	/// </summary>
	/// <param name="nextStName">次ステージのｐ名前</param>
	/// <param name="y">Y座標</param>
	/// <param name="angle">矢印角度(上基準の時計回り)</param>
	/// <param name="isReverseX">反転X</param>
	/// <param name="isReverxeY">反転Y/param>
	void DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX = false, bool isReverxeY = false);
	/// <summary>
	/// クリア条件のタイムを描画
	/// </summary>
	/// <param name="y">Y座標</param>
	/// <param name="handle">フォントハンドル</param>
	/// <param name="existTime">クリアタイム</param>
	void DrawTimeConditions(int y, int handle, int existTime);
	/// <summary>
	/// クリア条件の敵タイプを描画
	/// </summary>
	/// <param name="y">Y座標</param>
	/// <param name="handle">フォントハンドル</param>
	/// <param name="killedNum">倒される種類数</param>
	void DrawKilledConditions(int y, int handle, int killedNum);
	/// <summary>
	/// クリア条件の合計時間のやつ描画
	/// </summary>
	/// <param name="names">確認するステージ群情報</param>
	/// <param name="y">Y座標</param>
	/// <param name="handle">フォントハンドル</param>
	/// <param name="existTime">合計クリア時間</param>
	void DrawSumTimeConditions(const std::vector<std::string>& names, int y, int handle, int existTime);

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
	/// <param name="color">色</param>
	/// <param name="radius">半径 : def = 16</param>
	void DrawKilledEnemy(const std::string& enemyName, int addX, unsigned int color, int radius = 16) const;

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

	void ChangeSelectFunc();
	void ChangePlayingFunc();

private:
	/// <summary>
	/// ボスの死亡処理
	/// </summary>
	void BossDeath();

	/// <summary>
	/// 現在のタイムを描画
	/// </summary>
	/// <param name="x">X描画位置</param>
	/// <param name="y">Y描画位置</param>
	/// <param name="handle">フォントハンドル</param>
	void DrawTime(int x, int y, int handle);

	void DrawBestTime();

	/// <summary>
	/// 条件達成の描画
	/// </summary>
	/// <param name="y">Y描画位置</param>
	void DrawConditionsAchived(int y);

	/// <summary>
	/// 矢印の上に鍵の描画
	/// </summary>
	void DrawArrowLock(int x, int y, bool isBossStage, bool isClear) const;

	/// <summary>
	/// 壁の描画
	/// </summary>
	void DrawWall();

	/// <summary>
	/// 選択等の文字・画像描画
	/// </summary>
	/// /// <param name="cmd">コマンド名</param>
	/// <param name="str">ウェーブさせる文字列</param>
	/// <param name="num">文字列数</param>
	void DrawWave(const char* const cmd, const wchar_t* const str[], int num);

	int GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const;

protected:
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	GameManager& m_mgr;

	// 画像
	std::shared_ptr<FileBase> m_field;
	std::shared_ptr<FileBase> m_arrow;
	std::shared_ptr<FileBase> m_arrowFlash;
	std::shared_ptr<FileBase> m_arrowNo;
	std::shared_ptr<FileBase> m_arrowLock;
	std::shared_ptr<FileBase> m_arrowConditions;
	std::shared_ptr<FileBase> m_startFrame;
	std::unordered_map<std::string, std::shared_ptr<FileBase>> m_enemysImg;
	std::shared_ptr<FileBase> m_check;

	// 後ろのフレームを描画するよう
	std::shared_ptr<FileBase> m_bFrameImg;
	// サウンド
	std::shared_ptr<SoundSystem> m_sound;
	int m_soundFrame;
	// BGM
	std::shared_ptr<FileBase> m_selectBgm;
	std::shared_ptr<FileBase> m_playBgm;

	// 文字列を描画する用の画面ハンドル
	int m_strHandle;

	// ウィンドウサイズ
	const size& m_size;
	// フィールドサイズ
	float m_fieldSize;

	// ステージ変更可能までの待機時間
	const int kWaitChangeFrame = 30;
	const Vec2 m_centerPos;

	// ステージ名
	std::string m_stageName;

	// 達成の文字の描画用
	std::list<Achived> m_achived;

	// プレイヤー
	std::shared_ptr<Player> m_player;
	// 敵
	std::list<std::shared_ptr<EnemyBase>> m_enemy;
	// ボス
	std::shared_ptr<BossBase> m_boss;

	// 経過時間
	int m_frame;
	// 時間経過を行うかどうか
	bool m_isUpdateTime;

	// 待機時間
	int m_waitFrame;

	// 文字ウェーブ用の角度
	float m_waveAngle;
	// ウエーブのやつ描画するか
	bool m_isWaveDraw;

	// ベストタイム点滅
	bool m_isUpdateBestTime;


	// 
	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	const Input& m_input;
};

