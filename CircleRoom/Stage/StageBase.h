#pragma once
#include <vector>
#include <memory>
#include <string>

class StageManager;
class Player;
class EnemyBase;
class Input;
struct Size;

class StageBase
{
public:
	StageBase(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize);
	virtual ~StageBase();

	void Update(Input& input);
	void Draw();

	/// <summary>
	/// ステージ条件の描画
	/// </summary>
	virtual void DrawStageConditions();
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

protected:
	std::shared_ptr<StageManager> m_mgr;

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
};

