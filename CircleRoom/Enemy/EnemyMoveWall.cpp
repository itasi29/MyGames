#include "EnemyMoveWall.h"
#include <DxLib.h>
#include "Application.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 9.0f;
	// ���a
	constexpr float kRadius = 24.0f;

	// �J���[
	constexpr int kColor = 0x888888;
}

EnemyMoveWall::EnemyMoveWall(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "MoveWall";
	m_color = kColor;
}

EnemyMoveWall::~EnemyMoveWall()
{
}

void EnemyMoveWall::Init(const Vec2& vec)
{
	// �t���[���̏�����
	m_frame = 0;

	// ���a�̐ݒ�
	m_radius = kRadius;

	float centerX = m_size.w * 0.5f;
	float centerY = m_size.h * 0.5f;

	m_pos = Vec2{centerX + m_fieldSize * -vec.y, centerY};

	m_vec = vec * kSpeed;
}

void EnemyMoveWall::StartUpdate()
{
	m_frame++;
	m_pos += m_vec;
	Reflection(0.0f, false);

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyMoveWall::NormalUpdate()
{
	m_pos += m_vec;
	Reflection(0.0f, false);

	m_col.SetCenter(m_pos, m_radius);
}
