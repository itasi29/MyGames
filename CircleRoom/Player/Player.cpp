#include <DxLib.h>
#include "../Application.h"
#include "Player.h"
#include "../Common/Input.h"
#include "../Utility/Matrix3x3.h"

namespace
{
	// �����蔻��̔��a�̑傫��
	constexpr float kColRadius = 2.0f;

	// �v���C���[�̊e���_�܂ł̒���(�v���C���[�̑傫��)
	constexpr float kDistanceVertex = 24.0f;
	// �v���C���[�̃X�s�[�h
	constexpr float kSpeed = 4.0f;
	// �_�b�V�����̃X�s�[�h�{��
	constexpr float kDashSpeed = 4.0f;
	// �_�b�V���\����
	constexpr int kDashFrame = 50;
	// �_�b�V���ҋ@����
	constexpr int kDashWaitFrame = 25;

	// ��]�̍����s��
	Matrix4x4 kMatRight;
	Matrix4x4 kMatLeft;
	// ���`��Ԃ��s���t���[��
	constexpr int kInterpolatedFrame = 4;
}

Player::Player(Application& app) :
	m_app(app),
	m_size(m_app.GetWindowSize()),
	m_interpolatedFrame(-1),
	m_dashFrame(-1),
	m_dashWaitFrame(-1),
	m_isDash(false),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
	m_pos = Vec2{ m_size.w / 2.0f, m_size.h - 100.0f };

	kMatRight.AngleAxisZ(DX_PI_F / 3.0f * 2.0f);
	kMatLeft.AngleAxisZ(DX_PI_F / 3.0f * 4.0f);

	m_nowFront = Vec2::Up();
	m_frontVec = m_nowFront * kDistanceVertex;
	m_rightVec = kMatRight * m_nowFront * kDistanceVertex;
	m_leftVec = kMatLeft * m_nowFront * kDistanceVertex;
}

Player::~Player()
{
}

void Player::Update(Input& input)
{
	Move(input);
}

void Player::Draw()
{
	// �v���C���[���O�p�`�ŕ`��
	// �ォ�珇�ɐ��ʁA���A�E
	DrawTriangle(static_cast<int>(m_frontVec.x + m_pos.x), static_cast<int>(m_frontVec.y + m_pos.y),
		static_cast<int>(m_leftVec.x + m_pos.x), static_cast<int>(m_leftVec.y + m_pos.y),
		static_cast<int>(m_rightVec.x + m_pos.x), static_cast<int>(m_rightVec.y + m_pos.y),
		0xffffff, true);

	DrawFormatString(32, 32, 0xffffff, L"%.2f, %.2f", m_pos.x, m_pos.y);
	DrawFormatString(32, 32 + 16, 0xffffff, L"f:%.2f, %.2f", m_frontVec.x, m_frontVec.y);
	DrawFormatString(32, 32 + 32, 0xffffff, L"r:%.2f, %.2f", m_rightVec.x, m_rightVec.y);
	DrawFormatString(32, 32 + 48, 0xffffff, L"l:%.2f, %.2f", m_leftVec.x, m_leftVec.y);
}

void Player::Move(Input& input)
{
	// �[���x�N�g���ɖ߂�
	m_vec = Vec2::Zero();

	//m_vec = input.GetStickDate();

	if (input.IsPress("up"))
	{
		m_vec.y--;
	}
	if (input.IsPress("down"))
	{
		m_vec.y++;
	}
	if (input.IsPress("left"))
	{
		m_vec.x--;
	}
	if (input.IsPress("right"))
	{
		m_vec.x++;
	}

	// �ړ��x�N�g���𐳋K��
	m_vec.Normalize();

	Lerp();

	m_vec *= kSpeed;

	Dash(input);

	// ���W�Ɉړ��x�N�g���𑫂�
	m_pos += m_vec;
}

void Player::Dash(Input& input)
{
	// �_�b�V���ҋ@���Ԓ��Ȃ�ҋ@���Ԃ����炵�ď����I��
	if (m_dashWaitFrame > 0)
	{
		m_dashWaitFrame--;
		return;
	}

	// �_�b�V���R�}���h�������ꂽ��
	if (input.IsTriggered("dash"))
	{ 
		// �_�b�V������悤�ɂ���
		m_isDash = true;
		// �g�p���Ԃ̏�����
		m_dashFrame = kDashFrame;
	}
	
	// ���݃_�b�V�����Ȃ�
	if (m_isDash)
	{
		// �ړ��x�N�g���Ɍ��݌����Ă�������̒P�ʃx�N�g��*�X�s�[�h�������̂𑫂�
		m_vec += m_nowFront * kDashSpeed;
		// �g�p���Ԃ����炷
		m_dashFrame--;
		// �_�b�V������莞�ԉ��������邩��������I��
		if (m_dashFrame <= 0 || input.IsReleased("dash"))
		{
			m_isDash = false;
			// �ҋ@���Ԃ̏�����
			m_dashWaitFrame = kDashWaitFrame;
		}
	}	
}

void Player::Lerp()
{
	// �����Ă�������`��Ԃ��X�V����
	if (m_vec != m_nowFront)
	{
		m_interpolatedValue = m_vec - m_nowFront;
		m_interpolatedValue = m_interpolatedValue.GetNormalized() / kInterpolatedFrame;
		m_interpolatedFrame = kInterpolatedFrame;

		m_nowFront = m_vec;
	}

	// ���`��Ԃ����Ȃ��ꍇ�͕�ԏ������Ȃ�
	if (m_interpolatedFrame < 0) return;

	// ���`��Ԏ��Ԃ̍X�V
	m_interpolatedFrame--;
	// ���݂̐��ʕ����̍X�V
	//m_nowFront += m_interpolatedValue;

	//m_frontVec = m_nowFront * kDistanceVertex;
	//m_rightVec = kMatRight * m_nowFront * kDistanceVertex;
	//m_leftVec = kMatLeft * m_nowFront * kDistanceVertex;

	m_frontVec *= m_interpolatedValue;
	m_rightVec *= m_interpolatedValue;
	m_leftVec *= m_interpolatedValue;

	m_frontVec *= kDistanceVertex;
	m_rightVec *= kDistanceVertex;
	m_leftVec *= kDistanceVertex;
}
