#include <DxLib.h>
#include "../Application.h"
#include "EnemyNormal.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 4.0f;
	// ���a
	constexpr float kRadius = 16.0f;

	// ���߂̎��̉�����܂ł̃t���[��
	constexpr int kApeearFrame = 30;
}

EnemyNormal::EnemyNormal(const Size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
}

EnemyNormal::~EnemyNormal()
{
}

void EnemyNormal::Init(Vec2 pos)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;
	m_colPos.SetCenter(m_pos, m_radius);

	// �t���[���̏�����
	m_frame = 0;

	// �������������_���Ō��߂�
	float moveX = (GetRand(8) - 4) * 0.25f;
	float moveY = (GetRand(8) - 4) * 0.25f;
	m_vec = Vec2{ moveX, moveY };
	// ���K�����ăX�s�[�h�𒲐�
	m_vec.Normalize();
	m_vec *= kSpeed;
}

void EnemyNormal::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyNormal::NormalUpdate()
{
	m_pos += m_vec;
	Reflection();
}

void EnemyNormal::StartDraw()
{
	int alpha = 255 * (m_frame / kApeearFrame);
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), 0xff0000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EnemyNormal::NormalDraw()
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), 0xff0000, true);
}
