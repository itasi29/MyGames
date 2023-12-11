#include <DxLib.h>
#include <cmath>
#include <memory>

#include "Stage/StageBase.h"

#include "EnemyCreate.h"
#include "EnemyChild.h"

namespace
{
	// FIXME:�m��DxLib�Ń΂���`����Ă��͂������ǖY�ꂽ���玩���Œ�`���Ă���
	constexpr float kPai = 3.1415926535f;

	// �����X�s�[�h
	constexpr float kSpeed = 5.0f;
	// ���a
	constexpr float kRadius = 24.0f;

	// �J���[
	constexpr int kColor = 0xffffff;

	// 1�t���[�����Ƃ̊p�x
	constexpr float kRadian = 5.0f * (kPai / 180.0f);
	// �c�̗h�ꕝ
	constexpr float kSwingHeight = 2.0f;

	// �����Ԋu
	constexpr int kCreateFrame = 15;
}

EnemyCreate::EnemyCreate(const Size& windowSize, float fieldSize, StageBase* stage) :
	EnemyBase(windowSize, fieldSize),
	m_stage(stage),
	m_radian(0)
{
	m_name = "Create";
	m_color = kColor;
}

EnemyCreate::~EnemyCreate()
{
}

void EnemyCreate::Init(const Vec2& pos)
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

void EnemyCreate::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);

		m_frame = 0;
		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyCreate::NormalUpdate()
{
	m_radian += kRadian;
	if (m_radian > 2 * kPai)
	{
		m_radian = 0.0f;
	}

	// ���݂̃x�N�g���𐂒������ɃR�T�C���J�[�u�̒l�����炷
	m_conversionVec = m_vec * kSpeed + m_vec.Right() * cosf(m_radian) * kSwingHeight;

	// �o�����x�N�g��������
	m_pos += m_conversionVec;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);

	m_frame++;
	if (m_frame > kCreateFrame)
	{
		m_frame = 0;
		std::shared_ptr<EnemyChild> child;
		child = std::make_shared<EnemyChild>(m_windowSize, m_fieldSize);
		child->Init(m_pos);

		m_stage->GenericEnemy(child);
	}
}
