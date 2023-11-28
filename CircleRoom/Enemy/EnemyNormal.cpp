#include <DxLib.h>
#include "Application.h"
#include "EnemyNormal.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 4.0f;
	// ���a
	constexpr float kRadius = 16.0f;

	// ���߂̎��̉�����܂ł̃t���[��
	constexpr int kApeearFrame = 60;

	// �J���[
	constexpr int kColor = 0xffffff;
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
		m_rect.SetCenter(m_pos, m_radius);

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyNormal::NormalUpdate()
{
	m_pos += m_vec;
	Reflection();

	m_rect.SetCenter(m_pos, m_radius);
}

void EnemyNormal::StartDraw()
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kApeearFrame);
	int alpha = static_cast<int>(255 * rate);
	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), kColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EnemyNormal::NormalDraw()
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), kColor, true);

#ifdef _DEBUG
	// �����蔻��̕`��
	m_rect.Draw(0xff0000, false);
#endif

}
