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
	DrawCircle(m_pos.x, m_pos.y, m_radius, 
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
	// ���ꂼ��̔��a��2��̑����Z��
	// ���S�n���m�̒�����2����Z�����
	// �������Ă��邱�Ƃɂ���
	return (m_radius * m_radius + target.m_radius * target.m_radius) < (target.m_pos - m_pos).SqLength();
}
