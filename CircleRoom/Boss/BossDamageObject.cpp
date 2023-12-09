#include <DxLib.h>
#include "Application.h"
#include "BossDamageObject.h"

namespace
{
	// 半径
	constexpr float kRadius = 8.0f;

	// ランダム幅
	constexpr int kRand = 10;
}

BossDamageObject::BossDamageObject(const Size& windowSize, float fieldSize) :
	m_isUsed(false)
{
	// 中心地からフィールド範囲内に出現するようにランダムに設置
	m_pos.x = windowSize.w * 0.5f + GetRand(static_cast<int>(fieldSize * 2 - kRadius * 2)) - fieldSize - kRadius;
	m_pos.y = windowSize.h * 0.5f + GetRand(static_cast<int>(fieldSize * 2 - kRadius * 2)) - fieldSize - kRadius;

	m_col.SetCenter(m_pos, kRadius);
}

BossDamageObject::BossDamageObject(const Vec2& col) :
	m_isUsed(false)
{
	// 送られた値からランダムでずらして設置
	m_pos.x = col.x + GetRand(static_cast<int>(kRand * 0.5f)) - kRand;
	m_pos.y = col.y + GetRand(static_cast<int>(kRand * 0.5f)) - kRand;

	m_col.SetCenter(m_pos, kRadius);
}

BossDamageObject::~BossDamageObject()
{
}

void BossDamageObject::Draw()
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), static_cast<int>(kRadius), 0xff4500, true);
}
