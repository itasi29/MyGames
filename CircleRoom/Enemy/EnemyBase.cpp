#include <DxLib.h>
#include "../Application.h"
#include "EnemyBase.h"

namespace
{
	// �ǂ���̖@���x�N�g��
	const Vec2 kNorVecLeft  = Vec2{  1.0f,  0.0f };
	const Vec2 kNorVecRight = Vec2{ -1.0f,  0.0f };
	const Vec2 kNorVecUp    = Vec2{  0.0f,  1.0f };
	const Vec2 kNorVecDown  = Vec2{  0.0f, -1.0f };
}

EnemyBase::EnemyBase(const Size& windowSize, float fieldSize) :
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_radius(0),
	m_frame(0)
{
	m_updateFunc = &EnemyBase::StartUpdate;
	m_drawFunc = &EnemyBase::StartDraw;
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update()
{
	(this->*m_updateFunc)();
}

void EnemyBase::Draw()
{
	(this->*m_drawFunc)();
}

void EnemyBase::Reflection()
{
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

	// �ǂɓ��������甽���v����90�x��]
	// ��
	if (m_pos.x - m_radius < centerX - m_fieldSize)
	{
		m_pos.x = centerX - m_fieldSize + m_radius;
		ReflectionCal(kNorVecLeft);
	}
	// �E
	if (m_pos.x + m_radius > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize - m_radius;
		ReflectionCal(kNorVecRight);
	}
	// ��
	if (m_pos.y - m_radius < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize + m_radius;
		ReflectionCal(kNorVecUp);
	}
	// ��
	if (m_pos.y + m_radius > centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize - m_radius;
		ReflectionCal(kNorVecDown);
	}
}

void EnemyBase::ReflectionCal(Vec2 norVec)
{
	// �@���x�N�g���̒��������݂̃x�N�g���̒����Ɠ����ɂ���
	norVec = norVec * norVec.Dot(-m_vec);

	// �@���x�N�g����2�{���猻�݂̃x�N�g��������
	m_vec = m_vec + norVec * 2;
}

void EnemyBase::ChangeNormalFunc()
{
	m_updateFunc = &EnemyBase::NormalUpdate;
	m_drawFunc = &EnemyBase::NormalDraw;
}
