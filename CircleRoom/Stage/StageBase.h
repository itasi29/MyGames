#pragma once
#include <vector>
#include <memory>
#include <string>

class StageManager;
class Player;
class EnemyBase;
class Input;
struct Size;
struct StageData;

class StageBase
{
public:
	StageBase(StageManager& mgr, const Size& windowSize, float fieldSize);
	virtual ~StageBase();

	void Update(Input& input);
	void Draw();

	/// <summary>
	/// ステージのクリア確認
	/// </summary>
	virtual void CheckStageConditions() = 0;
	/// <summary>
	/// ステージ条件の描画
	/// </summary>
	/// <param name = "isPlaying">true:プレイ中, false:選択中</param>
	virtual void DrawStageConditions(bool isPlaying = false) = 0;
	/// <summary>
	/// 各ステージの初期化処理
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 敵の生成
	/// </summary>
	virtual void CreateEnemy() = 0;

	/// <summary>
	/// ステージを変更する
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <returns>true : 入れ替え可能/ fasle : 入れ替え不可能</returns>
	virtual void ChangeStage(Input& input) = 0;

	/// <summary>
	/// 現在のデータを保存する
	/// </summary>
	virtual void SaveInf() const = 0;

protected:
	// ステージ変更可能までの待機時間
	const int kWaitChangeFrame = 30;

	StageManager& m_mgr;

	// クリアデータテーブル
	std::vector<StageData> m_clearDataTable;

	// Windowサイズ
	const Size& m_windowSize;
	// フィールドサイズ
	float m_fieldSize;

	// ステージ名
	std::wstring m_stageName;

	// プレイヤー
	std::shared_ptr<Player> m_player;
	// 敵
	std::vector<std::shared_ptr<EnemyBase>> m_enemy;

	// 経過時間
	int m_frame;
	
	// 待機時間
	int m_waitFrame;

protected:
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)();

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	/// <summary>
	/// 選択中の更新処理
	/// </summary>
	/// <param name="input">入力情報</param>
	void UpdateSelect(Input& input);
	/// <summary>
	/// プレイ中の更新処理
	/// </summary>
	/// <param name="input">入力情報</param>
	void UpdatePlaying(Input& input);

	/// <summary>
	/// 選択中の描画処理
	/// </summary>
	void DrawSelect();
	/// <summary>
	/// プレイ中の描画処理
	/// </summary>
	void DrawPlaying();

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

private:
	/// <summary>
	/// 壁の描画
	/// </summary>
	void DrawWall();

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
	/// <param name="dir">進む方向</param>
	/// <param name="dir">進む方向の反対</param>
	/// /// <param name="nextStage">次のステージのポインタ</param>
	void ChangeClearData(int dir, int dirInversion, std::shared_ptr<StageBase>& nextStage) const;
};

