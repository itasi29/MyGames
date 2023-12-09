#include <DxLib.h>

#include "BossArmored.h"

namespace
{
	// FIXME:�m��DxLib�Ń΂���`����Ă��͂������ǖY�ꂽ���玩���Œ�`���Ă���
	constexpr float kPai = 3.1415927f;

	// �����X�s�[�h
	constexpr float kSpeed = 3.0f;
	// �h�ꕝ
	constexpr float kSwing = 1.5f;

	// ���a
	constexpr float kRadius = 24.0f;

	// �J���[
	constexpr int kColor = 0x08ff08;

	// 1�t���[�����Ƃ̊p�x
	constexpr float kRadian = 1.0f * (kPai / 180.0f);
}

BossArmored::BossArmored(const Size& windowSize, float fieldSize, int maxHp) :
	BossBase(windowSize, fieldSize, maxHp)
{
	m_name = "BossArmored";
	m_color = kColor;
}

BossArmored::~BossArmored()
{
}

void BossArmored::Init(Vec2& pos)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;

	// ���W�A���̏�����
	m_radian = 0;

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
	m_conversionVec = m_vec * kSpeed;
}

void BossArmored::OnAttack(const Collision& rect)
{
	m_radian = 0;

	m_hp--;

	// HP���[���ɂȂ����玀�S�Ƃ���
	if (m_hp <= 0)
	{
		// �o�[�̕`�����HP��0�ɂ��Ă���
		m_hp = 0;
		m_isExsit = false;
	}
}

void BossArmored::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);

		BossBase::ChangeNormalFunc();
	}
}

void BossArmored::NormalUpdate()
{
	m_radian += kRadian;

	if (m_radian > 2 * kPai)
	{
		m_radian = 0;
	}

	m_conversionVec.x = m_vec.x * cosf(m_radian);
	m_conversionVec.y = m_vec.y * sinf(m_radian);

	m_conversionVec = m_vec * kSpeed + m_conversionVec * kSwing;

	m_pos += m_conversionVec;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);
}
