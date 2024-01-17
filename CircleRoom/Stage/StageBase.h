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
	/// ステージのクリア確認
	/// </summary>
	virtual void CheckStageConditions() = 0;
	/// <summary>
	/// ステージ条件の描画
	/// </summary>
	/// <param name="drawY">描画する位置 書かなければ48が初期位置</param>
	/// <returns>Y軸の描画位置</returns>
	virtual int DrawStageConditions(int drawY = 48) = 0;
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

	/// <summary>
	/// 左に画面をスライドする処理
	/// </summary>
	/// <param name="nextStage">次のステージのshared_ptr</param>
	void SlideLeft(std::shared_ptr<StageBase> nextStage);
	/// <summary>
	/// 右に画面をスライドする処理
	/// </summary>
	/// <param name="nextStage">次のステージのshared_ptr</param>
	void SlideRight(std::shared_ptr<StageBase> nextStage);
	/// <summary>
	/// 上に画面をスライドする処理
	/// </summary>
	/// <param name="nextStage">次のステージのshared_ptr</param>
	void SlideUp(std::shared_ptr<StageBase> nextStage);
	/// <summary>
	/// 下に画面をスライドする処理
	/// </summary>
	/// <param name="nextStage">次のステージのshared_ptr</param>
	void SlideDown(std::shared_ptr<StageBase> nextStage);

	void ChangeSelectFunc();
	void ChangePlayingFunc();

private:
	/// <summary>
	/// ボスの死亡処理
	/// </summary>
	void BossDeath();

	/// <summary>
	/// スライド処理の全体共通処理
	/// </summary>
	/// <param name="now">現在の画面を保存するための画面「ハンドル</param>
	/// <param name="next">次の画面を保存するための画面ハンドル</param>
	/// <param name="nextStage">次のステージのポインタ</param>
	void SlideStart(int& now, int& next, const std::shared_ptr<StageBase>& nextStage);

	/// <summary>
	/// 進む方向先のクリア情報もクリアしているとする
	/// </summary>
	/// /// <param name="nextStage">次のステージのポインタ</param>
	void ChangeClearData(const std::shared_ptr<StageBase>& nextStage) const;

	/// <summary>
	/// 壁の描画
	/// </summary>
	void DrawWall();

protected:
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	GameManager& m_mgr;

	// 文字列を描画する用の画面ハンドル
	int m_strHandle;

	// 後ろのフレームを描画するよう
	std::shared_ptr<FileBase> m_bFrameImg;

	// ウィンドウサイズ
	const size& m_size;
	// フィールドサイズ
	float m_fieldSize;

	// ステージ変更可能までの待機時間
	const int kWaitChangeFrame = 30;
	const Vec2 m_centerPos;

	// ステージ名
	std::string m_stageName;

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
};

