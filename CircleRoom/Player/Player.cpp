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
	constexpr float kDashAdd = 2.0f;
	// �_�b�V���\����
	constexpr int kDashFrame = 50;
	// �_�b�V���ҋ@����
	constexpr int kDashWaitFrame = 25;

	// ��]�̍����s��(60�x)
	Matrix3x3 kMatAngle;
	// ���`��Ԃ��s���t���[��
	constexpr int kInterpolatedFrame = 4;
}

Player::Player(Application& app) :
	m_app(app),
	m_size(m_app.GetWindowSize()),
	m_interpolatedFrame(0),
	m_dashFrame(0),
	m_dashWaitFrame(0),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
	m_pos = Vec2{ m_size.w / 2.0f, m_size.h - 100.0f };

	kMatAngle.Angle(DX_PI_F / 3.0f * 2.0f);

	m_front = Vec2::Up() * kDistanceVertex;
	m_nowFront = m_front;
}

Player::~Player()
{
}

void Player::Update(Input& input)
{
	Move(input);
	Dash(input);

	// ���`��Ԃ̊Ԃ͏������s��
	if (m_interpolatedFrame > 0)
	{
		m_interpolatedFrame--;
		m_nowFront += m_interpolatedValue;
	}
}

void Player::Draw()
{
	// �v���C���[���O�p�`�ŕ`��
	// �ォ�珇�ɐ��ʁA���A�E
	Vec2 right = kMatAngle * m_nowFront;
	Vec2 left = kMatAngle * right;
	DrawTriangle(static_cast<int>(m_nowFront.x + m_pos.x), static_cast<int>(m_nowFront.y + m_pos.y),
		static_cast<int>(left.x + m_pos.x), static_cast<int>(left.y + m_pos.y),
		static_cast<int>(right.x + m_pos.x), static_cast<int>(right.y + m_pos.y),
		0xffffff, true);

	DrawFormatString(32, 32, 0xffffff, L"%.2f, %.2f", m_pos.x, m_pos.y);
}

void Player::Move(Input& input)
{
	// �[���x�N�g���ɖ߂�
	m_vec = Vec2::Zero();

	m_vec = input.GetStickDate();

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

	// �[���x�N�g���Ȃ�ȍ~�̏������s��Ȃ�
	if (m_vec.SqLength() == 0)	return;

	// �ړ��x�N�g���𐳋K��
	m_vec.Normalize();

	// �����̍X�V
	m_front = m_vec * kDistanceVertex;
	// �ς���������܂ł̐��`��Ԃ��쐬
	m_interpolatedValue = (m_front - m_nowFront) / kInterpolatedFrame;
	m_interpolatedFrame = kInterpolatedFrame;

	m_vec *= kSpeed;

	// ���W�Ɉړ��x�N�g���𑫂�
	m_pos += m_vec;
}

void Player::Dash(Input& input)
{
	// todo:�_�b�V���̘A���g�p�s�ƃt���[���̍X�V�������

	if (m_dashFrame > 0 && input.IsPress("dash"))
	{
		m_dashFrame--;
		m_vec *= kDashAdd;
	}
	// �����ꂽ�Ƃ������Ԍo�߂����Ƃ�
	else
	{

	}
}
