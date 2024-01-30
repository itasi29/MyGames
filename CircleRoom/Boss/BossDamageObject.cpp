#include <DxLib.h>
#include "Application.h"
#include "BossDamageObject.h"

namespace
{
	// 半径
	constexpr float kRadius = 16.0f;

	// ランダム幅
	constexpr int kRand = 10;

	// フラッシュ間隔
	constexpr int kFlashInterval = 30;
}

BossDamageObject::BossDamageObject(const size& windowSize, float fieldSize) :
	m_isUsed(false),
	m_flashFrame(0)
{
	// 中心地からフィールド範囲内に出現するようにランダムに設置
	m_pos.x = windowSize.w * 0.5f + GetRand(static_cast<int>(fieldSize * 2 - kRadius * 2)) - fieldSize + kRadius;
	m_pos.y = windowSize.h * 0.5f + GetRand(static_cast<int>(fieldSize * 2 - kRadius * 2)) - fieldSize + kRadius;

	m_col.SetCenter(m_pos, kRadius);
}

BossDamageObject::BossDamageObject(const Vec2& col) :
	m_isUsed(false),
	m_flashFrame(0)
{
	// 送られた値からランダムでずらして設置
	m_pos.x = col.x + GetRand(static_cast<int>(kRand * 0.5f)) - kRand;
	m_pos.y = col.y + GetRand(static_cast<int>(kRand * 0.5f)) - kRand;

	m_col.SetCenter(m_pos, kRadius);
}

BossDamageObject::~BossDamageObject()
{
}

void BossDamageObject::Update()
{
	m_flashFrame++;
}

void BossDamageObject::Draw()
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), static_cast<int>(kRadius), 0xd2001a, true);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), static_cast<int>(kRadius * 0.5), 0xff4500, true);

	if ((m_flashFrame / kFlashInterval) % 2 == 0)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), static_cast<int>(kRadius), 0xff4500, false, 2);
	}
}
