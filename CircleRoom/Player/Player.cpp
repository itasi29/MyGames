#include "Player.h"
#include <DxLib.h>
#include "../Common/Input.h"
#include "../Application.h"

namespace
{
	// �����蔻��̔��a�̑傫��
	constexpr float kColRadius = 2.0f;

	// �v���C���[�̊e���_�܂ł̒���(�v���C���[�̑傫��)
	constexpr float kDistanceVertex = 5.0f;
	// �v���C���[�̃X�s�[�h
	constexpr float kSpeed = 4.0f;
}

Player::Player(Application& app) :
	m_app(app),
	m_size(m_app.GetWindowSize()),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_pos = Vec2{ m_app.GetWindowSize().w / 2, m_app.GetWindowSize().h - 100.0f };
}

void Player::Update(Input& input)
{
	Move(input);
}

void Player::Draw()
{
	// ����f�o�b�O�p�ŉ~�̕`��
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 8, 0xffffff, true);
}

void Player::Move(Input& input)
{
	// �[���x�N�g���ɖ߂�
	m_vec = Vec2::zero();

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
	// �X�s�[�h�ɕύX
	m_vec *= kSpeed;

	// ���W�Ɉړ��x�N�g���𑫂�
	m_pos += m_vec;
}
