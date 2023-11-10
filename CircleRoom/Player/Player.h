#pragma once
#include "../Utility/Vec2.h"
#include "../Utility/Rect.h"
class Input;
class Player
{
public:
	Player();
	~Player();

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
	Rect GetColPos() const { return m_colRect; }
	/// <summary>
	/// 当たり判定の半径を取得
	/// </summary>
	/// <returns>当たり判定の半径</returns>
	float GetColRadius() const { return m_colRaidus; }

private:
	// 中心座標
	Vec2 m_pos;
	// 移動ベクトル
	Vec2 m_vec;

	// 当たり判定
	Rect m_colRect;
	// 当たり判定の半径
	float m_colRaidus;

	// 生存しているか
	bool m_isExsit;

	void Move();
};

