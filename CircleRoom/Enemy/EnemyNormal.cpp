#include <DxLib.h>
#include "Application.h"
#include "EnemyNormal.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 5.0f;
	// ���a
	constexpr float kRadius = 24.0f;

	// �J���[
	constexpr int kColor = 0xffffff;
}

EnemyNormal::EnemyNormal(const Size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "Normal";
	m_color = kColor;
}

EnemyNormal::~EnemyNormal()
{
}

void EnemyNormal::Init(const Vec2& pos)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;

	// �t���[���̏�����
	m_frame = 0;

	// �������������_���Ō��߂�
	float moveX = (GetRand(16) - 8) * 0.125f;
	float moveY = (GetRand(16) - 8) * 0.125f;
	m_vec = Vec2{ moveX, moveY };

	// �[���x�N�g���łȂ��Ȃ琳�K��
	if (m_vec.SqLength() > 0)
	{
		m_vec.Normalize();
	}
	// �[���x�N�g���Ȃ������^���ɂ���
	else
	{
		m_vec = Vec2{ 1.0f, 0.0f };
	}

	// �X�s�[�h�𒲐�
	m_vec *= kSpeed;
}

void EnemyNormal::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyNormal::NormalUpdate()
{
	m_pos += m_vec;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);
}