#include "Rect.h"
#include <DxLib.h>
#include <cassert>

Rect::Rect() :
	m_radius(0)
{
}

Rect::~Rect()
{
}

void Rect::Draw(unsigned int color, bool isFill) const
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), static_cast<int>(m_radius),
		color, isFill);
}

void Rect::SetCenter(Vec2 pos, float radius)
{
	m_pos = pos;
	m_radius = radius;
}

Vec2 Rect::GetCenter() const
{
	return m_pos;
}

bool Rect::IsCollsion(const Rect& target)
{
	// それぞれの半径の2乗の足し算が
	// 中心地同士の長さの2乗より短ければ
	// 当たっていることにする
	return (m_radius * m_radius + target.m_radius * target.m_radius) < (target.m_pos - m_pos).SqLength();
}
