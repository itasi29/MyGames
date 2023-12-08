#pragma once
#include <string>
#include "Vec2.h"
#include "Rect.h"

struct Size;

class BossBase
{
public:
	BossBase(const Size& windowSize, float fieldSize, int hp);
	virtual ~BossBase();

	virtual void Init(Vec2& pos) = 0;
	void Update();
	void Draw();

	Rect GetRect() const { return m_rect; }

	/// <summary>
	/// HPを1減らす
	/// </summary>
	virtual void OnHp();

protected:
	// メンバ関数ポインタ
	using updateFunc_t = void(BossBase::*)();
	using drawFunc_t = void(BossBase::*)();

	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;

	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;

	virtual void StartDraw();
	virtual void NormalDraw();

	/// <summary>
	/// HPバーの描画
	/// </summary>
	void DrawHpBar();

protected:
	// スクリーンサイズ
	const Size& m_windoSize;
	// フィールドサイズ
	const float m_fieldSize;

	// 敵の名前
	std::string m_name;
	// 最大体力
	const int m_maxHp;
	// 体力
	int m_hp;

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

