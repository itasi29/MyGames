#include "EnemySplitTwoBound.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 1.5f;
	// ���a
	constexpr float kRadius = 12.0f;

	// �J���[
	constexpr int kColor = 0xffffff;

	// �o�E���h��
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
		// �����œn���ꂽ�ʒu�������ʒu��
		m_pos = pos;
		m_radius = kRadius;

		// �ړ��x�N�g���𒲐�
		m_vec = vec;
		m_vec *= kSpeed;

		// �t���[���̏�����
		m_frame = 0;

		// ���߂��画�������
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
