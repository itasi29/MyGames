#include "Player.h"

namespace
{
	// �����蔻��̔��a�̑傫��
	constexpr float kColRadius = 2.0f;
}

Player::Player() :
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
}

Player::~Player()
{
}

void Player::Update(Input& input)
{
	Move();
}

void Player::Draw()
{
}

void Player::Move()
{

}
