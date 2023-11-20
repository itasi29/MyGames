#pragma once

#include "../Utility/Vec2.h"
#include "../Utility/Rect.h"

class Size;

/// <summary>
/// Enemyクラスの基底
/// </summary>
class EnemyBase
{
public:
	EnemyBase(const Size& windowSize, float fieldSize);
	virtual ~EnemyBase();

	virtual void Update() = 0;
	virtual void Draw();

protected:
	// スクリーンサイズ
	const Size& m_windowSize;
	// フィールドのサイズ
	float m_fieldSize;

	// 中心座標
	Vec2 m_pos;
	// 移動ベクトル
	Vec2 m_vec;
	// 当たり判定
	Rect m_colPos;
	// 半径
	float m_radius;
	// 生存判定
	bool m_isExsit;

protected:
	virtual void Reflection();
};

