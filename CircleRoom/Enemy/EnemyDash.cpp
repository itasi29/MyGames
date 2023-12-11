#include <DxLib.h>
#include "Application.h"

#include "EnemyDash.h"

#include "Player/Player.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 5.0f;
	// ���a
	constexpr float kRadius = 24.0f;

	// �J���[
	constexpr int kColor = 0x0808ff;

	// �_�b�V�����̃X�s�[�h
	constexpr float kDashSpeed = 15.0f;
	// �_�b�V���J�n�܂ł̃X�s�[�h
	constexpr float kStartWaitSpeed = 2.5f;

	// �_�b�V���ҋ@����
	constexpr int kWaitDashFrame = 60 * 5;
	// �_�b�V�����J�n����܂ł̃t���[��
	constexpr int kStartWaitDashFrame = 30;
}

EnemyDash::EnemyDash(const Size& windowSize, float fieldSize, std::shared_ptr<Player>& player) :
	EnemyBase(windowSize, fieldSize),
	m_player(player),
	m_isDash(false),
	m_waitDashFrame(kWaitDashFrame),
	m_startWaitDashFrame(kStartWaitDashFrame)
{
	m_name = "Dash";
	m_color = kColor;
}

EnemyDash::~EnemyDash()
{
}

void EnemyDash::Init(const Vec2& pos)
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

void EnemyDash::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyDash::NormalUpdate()
{
	DashStartProcess();
	m_pos += m_vec;

	// ���˂������A�_�b�V����ԂȂ�ʏ��Ԃɖ߂�
	if (Reflection() && m_isDash)
	{
		// �_�b�V���I��
		m_isDash = false;

		// �_�b�V����Ԃ���ʏ푬�x�ɖ߂�
		m_vec = m_vec.GetNormalized() * kSpeed;

		// �ҋ@���Ԃ�߂�
		m_waitDashFrame = kWaitDashFrame;
		// �X�^�[�g�ҋ@���Ԃ�߂�
		m_startWaitDashFrame = kStartWaitDashFrame;
	}

	m_col.SetCenter(m_pos, m_radius);
}

void EnemyDash::DashStartProcess()
{
	// �_�b�V�����Ȃ珈�����Ȃ�
	if (m_isDash)
	{
		return;
	}

	// �ҋ@���ԑ҂�����
	if (m_waitDashFrame > 0)
	{
		m_waitDashFrame--;

		// �ҋ@���Ԃ��I�������
		if (m_waitDashFrame <= 0)
		{
			// ���K��
			m_vec.Normalize();
			// ���x�̕ύX
			m_vec *= kStartWaitSpeed;
		}
		return;
	}
	
	// �X�^�[�g�܂ł̃t���[�������炷
	m_startWaitDashFrame--;

	// �X�^�[�g�ҋ@���I�������
	if (m_startWaitDashFrame <= 0)
	{
		m_isDash = true;

		// ���݂̈ʒu����v���C���[�܂ł̃x�N�g���̐���
		m_vec = m_player->GetPos() - m_pos;
		// ���K��
		m_vec.Normalize();
		// �X�s�[�h�ɕϊ�
		m_vec *= kDashSpeed;
	}
}
