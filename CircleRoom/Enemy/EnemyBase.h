#pragma once

#include "Vec2.h"
#include "Rect.h"

struct Size;

/// <summary>
/// Enemyクラスの基底
/// </summary>
class EnemyBase
{
public:
	EnemyBase(const Size& windowSize, float fieldSize);
	virtual ~EnemyBase();

	virtual void Init(Vec2 pos) = 0;
	void Update();
	void Draw();

	/// <summary>
	/// 当たり判定の中心座標を取得
	/// </summary>
	/// <returns>当たり判定の中心座標</returns>
	Rect GetRect() const { return m_rect; }

protected:
	/// <summary>
	/// 壁に当たったら反射させる
	/// </summary>
	virtual void Reflection();
	/// <summary>
	/// 反射させる計算
	/// </summary>
	void ReflectionCal(Vec2 norVec);
	void ShiftReflection(Vec2 shift);

	// normal関数に変更する関数
	void ChangeNormalFunc();

protected:
	// メンバ関数ポインタ
	using updateFunc_t = void(EnemyBase::*)();
	using drawFunc_t = void(EnemyBase::*)();

	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;

	// 更新関数
	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;

	// 描画関数
	virtual void StartDraw() = 0;
	virtual void NormalDraw() = 0;

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
	Rect m_rect;
	// 半径
	float m_radius;

	// フレーム
	int m_frame;
};

