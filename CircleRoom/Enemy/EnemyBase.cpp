#include <DxLib.h>
#include "EnemyBase.h"

EnemyBase::EnemyBase(const Size& windowSize, float fieldSize) :
	m_windowSize(windowSize),
	m_fieldSize(fieldSize)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Draw()
{
	DrawCircle(m_pos.x, m_pos.y, m_radius, 0xff0000, true);
}

void EnemyBase::Reflection()
{
}
