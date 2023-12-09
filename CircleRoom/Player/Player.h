#pragma once
#include "Vec2.h"
#include "Utility/Collision.h"

class Input;
class Application;
struct Size;

class Player
{
public:
	Player(const Size& windowSize, float fieldSize);
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();
	void Update(Input& input);
	void Draw();

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	/// <returns>中心座標</returns>
	Vec2 GetPos() const { return m_pos; }

	/// <summary>
	/// 当たり判定の中心座標を取得
	/// </summary>
	/// <returns>当たり判定の中心座標</returns>
	Collision GetRect() const { return m_col; }
	bool IsDash() const { return m_isDash; }
	/// <summary>
	/// 生存しているか
	/// </summary>
	/// <returns>生存判定</returns>
	bool IsExsit() const { return m_isExsit; }

	/// <summary>
	/// 死亡処理
	/// </summary>
	void Death();

private:
	// スクリーンサイズ
	const Size& m_windowSize;
	// フィールドのサイズ
	float m_fieldSize;

	// 中心座標
	Vec2 m_pos;
	// 正面ベクトル
	Vec2 m_frontVec;
	// 右ベクトル
	Vec2 m_rightVec;
	// 左ベクトル
	Vec2 m_leftVec;
	// 現在の正面方向
	Vec2 m_nowFront;
	// 移動ベクトル
	Vec2 m_vec;

	// 当たり判定
	Collision m_col;

	// ダッシュするフレーム
	int m_dashFrame;
	// ダッシュ再利用可能までのフレーム
	int m_dashWaitFrame;
	// ダッシュ判定
	bool m_isDash;

	// 生存しているか
	bool m_isExsit;

private:
	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	/// <param name="input">入力情報</param>
	void Move(Input& input);
	/// <summary>
	/// ダッシュ処理
	/// </summary>
	/// <param name="input">入力情報</param>
	void Dash(Input& input);
	/// <summary>
	/// 場外判定処理
	/// </summary>
	void InRange();
};

