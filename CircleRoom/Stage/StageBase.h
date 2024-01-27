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

	/// <summary>
	/// 選択中の描画処理
	/// </summary>
	void DrawSelect();
	/// <summary>
	/// プレイ中の描画処理
	/// </summary>
	void DrawPlaying();

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
	/// 左矢印の描画
	/// </summary>
	void DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName) const;
	/// <summary>
	/// 右矢印の描画
	/// </summary>
	void DrawRightArrow(bool isAlreadyClear, const std::string& nextStName) const;
	/// <summary>
	/// 上矢印の描画
	/// </summary>
	void DrawUpArrow(bool isAlreadyClear, const std::string& nextStName) const;
	/// <summary>
	/// 下矢印の描画
	/// </summary>
	void DrawDownArrow(bool isAlreadyClear, const std::string& nextStName) const;

	void DrawKilledEnemy(const std::string& enemyName, int addX, unsigned int color, int radius = 16) const;

	/// <summary>
	/// クリアした文字の描画群に追加
	/// </summary>
	/// <param name="dir">方向</param>
	void AddAchivedStr(const std::wstring& dir);

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

	/// <summary>
	/// 条件達成の描画
	/// </summary>
	void DrawConditionsAchived();

	/// <summary>
	/// 壁の描画
	/// </summary>
	void DrawWall();

	/// <summary>
	/// 選択等の画像の描画
	/// </summary>
	void DrawImage();

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
	std::shared_ptr<FileBase> m_arrowConditions;
	std::shared_ptr<FileBase> m_startFrame;

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

	// 
	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	const Input& m_input;
};

