#pragma once
#include "Vec2.h"
#include "Collision.h"

struct Size;

/// <summary>
/// ボスにダメージを与える物体
/// </summary>
class BossDamageObject
{
public:
	BossDamageObject(const Size& windowSize, float fieldSize);
	BossDamageObject(const Vec2& col);
	~BossDamageObject();

	void Draw();

	/// <summary>
	/// 当たり判定の中心座標を取得
	/// </summary>
	/// <returns>当たり判定の中心座標</returns>
	Collision GetRect() const { return m_col; }

	/// <summary>
	/// 使ったことにする
	/// </summary>
	void Used() { m_isUsed = true; }

	/// <summary>
	/// 既に使用済みになっていないか
	/// </summary>
	/// <returns>true: 使用済み / false:未使用</returns>
	bool IsUsed() const { return m_isUsed; }

private:
	Vec2 m_pos;
	Collision m_col;

	bool m_isUsed;
};

