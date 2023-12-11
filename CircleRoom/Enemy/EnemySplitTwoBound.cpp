#include "EnemySplitTwoBound.h"

namespace
{
	// 動くスピード
	constexpr float kSpeed = 3.0f;
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
