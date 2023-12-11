#include "EnemySplitTwoBound.h"

namespace
{
	// 動くスピード
	constexpr float kSpeed = 1.5f;
	// 半径
	constexpr float kRadius = 12.0f;

	// カラー
	constexpr int kColor = 0xffffff;

	// バウンド回数
	constexpr int kBoundNum = 2;
}

EnemySplitTwoBound::EnemySplitTwoBound(const Size& windowSize, float fieldSize) :
	EnemySplit(windowSize, fieldSize),
	m_boundNo(0)
{
	m_name = "SplitTwoBound";
	m_color = kColor;
}

EnemySplitTwoBound::~EnemySplitTwoBound()
{
}

void EnemySplitTwoBound::Init(const Vec2& pos, Vec2& vec)
{
		// 引数で渡された位置を初期位置に
		m_pos = pos;
		m_radius = kRadius;

		// 移動ベクトルを調整
		m_vec = vec;
		m_vec *= kSpeed;

		// フレームの初期化
		m_frame = 0;

		// 初めから判定を持つ
		m_col.SetCenter(m_pos, m_radius);

		ChangeNormalFunc();
}

void EnemySplitTwoBound::NormalUpdate()
{
	m_pos += m_vec;
	if (Reflection())
	{
		m_boundNo++;

		if (m_boundNo > kBoundNum)
		{
			m_isExsit = false;
		}
	}

	m_col.SetCenter(m_pos, m_radius);
}
