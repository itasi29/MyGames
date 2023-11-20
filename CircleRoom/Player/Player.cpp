#include <DxLib.h>
#include "../Application.h"
#include "Player.h"
#include "../Common/Input.h"

namespace
{
	// �����蔻��̔��a�̑傫��
	constexpr float kColRadius = 10.0f;

	// �v���C���[�̑傫��
	constexpr float kSize = 24.0f;
	// �v���C���[�̃X�s�[�h
	constexpr float kSpeed = 4.0f;
	// �v���C���[�̒��S���画����ǂ̂��炢��������
	constexpr float kColShift = kSize * 0.32f;

	// �_�b�V�����̃X�s�[�h�{��
	constexpr float kDashSpeed = 4.0f;
	// �_�b�V���\����
	constexpr int kDashFrame = 50;
	// �_�b�V���ҋ@����
	constexpr int kDashWaitFrame = 25;

	// ���`��Ԃ��s���t���[��
	constexpr int kInterpolatedFrame = 50;
}

Player::Player(const Size& windowSize, float fieldSize) :
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_dashFrame(-1),
	m_dashWaitFrame(-1),
	m_isDash(false),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
	m_pos = Vec2{ m_windowSize.w / 2.0f, m_windowSize.h - fieldSize };

	m_nowFront = Vec2::Up();
	m_frontVec = m_nowFront * kSize;
	m_rightVec = m_nowFront.Right() * kSize * 0.5f;
	m_leftVec = m_nowFront.Left() * kSize * 0.5f;

	m_colPos.SetCenter(m_pos, kColRadius, m_nowFront.x * kColShift, m_nowFront.y * kColShift);
}

Player::~Player()
{
}

void Player::Update(Input& input)
{
	Move(input);
	InRange();

	// �����蔻��̍X�V
	m_colPos.SetCenter(m_pos, kColRadius, m_nowFront.x * kColShift, m_nowFront.y * kColShift);
}

void Player::Draw()
{
	// �v���C���[���O�p�`�ŕ`��
	// �ォ�珇�ɐ��ʁA���A�E
	DrawTriangle(static_cast<int>(m_frontVec.x + m_pos.x), static_cast<int>(m_frontVec.y + m_pos.y),
		static_cast<int>(m_leftVec.x + m_pos.x), static_cast<int>(m_leftVec.y + m_pos.y),
		static_cast<int>(m_rightVec.x + m_pos.x), static_cast<int>(m_rightVec.y + m_pos.y),
		0xffffff, true);

	/*�ȉ��f�o�b�O�p*/
#ifdef _DEBUG
	// �����蔻��`��
	//m_colPos.Draw(0xff0000, false);

	DrawFormatString(32, 32, 0xffffff, L"pos;%.2f, %.2f", m_pos.x, m_pos.y);
#endif
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

	// �ړ��x�N�g���𐳋K��
	m_vec.Normalize();

	if (m_vec.SqLength() > 0)
	{
		m_nowFront = m_vec;
		m_frontVec = m_nowFront * kSize;
		m_rightVec = m_nowFront.Right() * kSize * 0.5f;
		m_leftVec = m_nowFront.Left() * kSize * 0.5f;
	}

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

	// ���݃_�b�V�����łȂ��Ȃ珈���I��
	if (!m_isDash) return;
	
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

void Player::InRange()
{
	float centerX = m_windowSize.w *0.5f;
	float centerY = m_windowSize.h * 0.5f;

	// ������
	// ���݂̈ʒu���琳�ʕ������̌����ɒ��S�����炵(�����蔻��̈ʒu�Ɏ����Ă���)
	// �����蔻��̔��a�����炵���ʒu����O�ňʒu�����炷
	if (m_pos.x + m_nowFront.x * kColShift - kColRadius < centerX - m_fieldSize)
	{
		// ��O�̒[���瓖���蔻��̈ʒu�����炵
		// ���a���E�ɂ��炷
		m_pos.x = centerX - m_fieldSize - m_nowFront.x * kColShift + kColRadius;
	}
	// �E����
	if (m_pos.x + m_nowFront.x * kColShift + kColRadius > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize - m_nowFront.x * kColShift - kColRadius;
	}
	// �㏈��
	if (m_pos.y + m_nowFront.y * kColShift - kColRadius < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize - m_nowFront.y * kColShift + kColRadius;
	}
	// ������
	if (m_pos.y + m_nowFront.y * kColShift + kColRadius> centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize - m_nowFront.y * kColShift - kColRadius;
	}
}
