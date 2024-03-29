#include "Utility/Collision.h"
#include <DxLib.h>
#include <cassert>

Collision::Collision() :
	m_radius(0)
{
}

Collision::~Collision()
{
}

void Collision::Draw(unsigned int color, bool isFill) const
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), static_cast<int>(m_radius),
		color, isFill);
}

void Collision::SetCenter(Vec2 pos, float radius, float shiftX, float shiftY)
{
	m_pos = pos;
	m_pos.x += shiftX;
	m_pos.y += shiftY;
	m_radius = radius;
}

bool Collision::IsCollsion(const Collision& target) const
{
	// それぞれの半径の2乗の足し算が
	// 中心地同士の長さの2乗より短ければ
	// 当たっていることにする
	float dis = (target.m_pos - m_pos).SqLength();
	float sumRadius = (target.m_radius + m_radius) * (m_radius + target.m_radius);

	return (sumRadius > dis);
}
