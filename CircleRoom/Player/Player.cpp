#include <DxLib.h>
#include "../Application.h"
#include "Player.h"
#include "../Common/Input.h"

namespace
{
	// �����蔻��̔��a�̑傫��
	constexpr float kColRadius = 2.0f;

	// �v���C���[�̑傫��
	constexpr float kSize = 24.0f;
	// �v���C���[�̃X�s�[�h
	constexpr float kSpeed = 4.0f;
	// �_�b�V�����̃X�s�[�h�{��
	constexpr float kDashSpeed = 4.0f;
	// �_�b�V���\����
	constexpr int kDashFrame = 50;
	// �_�b�V���ҋ@����
	constexpr int kDashWaitFrame = 25;

	// ���`��Ԃ��s���t���[��
	constexpr int kInterpolatedFrame = 50;
}

Player::Player(Application& app) :
	m_app(app),
	m_size(m_app.GetWindowSize()),
	m_interpolatedFrame(-1),
	m_interpolatedFrameNum(0),
	m_dashFrame(-1),
	m_dashWaitFrame(-1),
	m_isDash(false),
	m_colRaidus(kColRadius),
	m_isExsit(false)
{
	m_pos = Vec2{ m_size.w / 2.0f, m_size.h - 100.0f };

	m_nowFront = Vec2::Up();
	m_frontVec = m_nowFront * kSize;
	m_rightVec = m_nowFront.Right() * kSize * 0.5f;
	m_leftVec = m_nowFront.Left() * kSize * 0.5f;

	m_lastChangeVec = m_nowFront;
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

	// �v���C���[�̒��S��\��
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 3, 0xff00ff, true);

	DrawFormatString(32, 32, 0xffffff, L"pos;%.2f, %.2f", m_pos.x, m_pos.y);
	DrawFormatString(32, 32+16, 0xff0000, L"vec:%.2f, %.2f", m_vec.x, m_vec.y);
	DrawFormatString(32, 32+32, 0xff0000, L"now:%.2f, %.2f", m_nowFront.x, m_nowFront.y);

	int centerX = m_size.w * 0.5f;
	int centerY = m_size.h * 0.5f;
	int lineLong = 30;
	DrawLine(centerX, centerY,
		centerX + m_lastChangeVec.x * lineLong, centerY + m_lastChangeVec.y * lineLong,
		0x0000ff, 2);
	DrawLine(centerX, centerY,
		centerX + m_firstChangeVec.x * lineLong, centerY + m_firstChangeVec.y * lineLong,
		0xff0000, 2);
	DrawLine(centerX, centerY,
		centerX + m_nowFront.x * lineLong, centerY + m_nowFront.y * lineLong,
		0x00ff00, 2);
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

#if true
	Lerp();
#else
	if (m_vec.SqLength() > 0)
	{
		m_nowFront = m_vec;
		m_frontVec = m_nowFront * kSize;
		m_rightVec = m_nowFront.Right() * kSize * 0.5f;
		m_leftVec = m_nowFront.Left() * kSize * 0.5f;
	}
#endif

	m_vec *= kSpeed;

	Dash(input);

	// ���W�Ɉړ��x�N�g���𑫂�
	m_pos += m_vec;
}

void Player::Lerp()
{
	// �����Ă�������`��Ԃ��X�V����
	if (m_vec != m_lastChangeVec && m_vec.SqLength() > 0)
	{
#if false
		// ��_�Ԃ̍��{2���Ԃɂ����鎞�ԂƂ���
		int num = abs(m_vec.x - m_nowFront.x + m_vec.y - m_nowFront.y)*5 + 2;
		// ��Ԃ̎��ԁ{�����炠������Ԃ̎��Ԃ�����
		m_interpolatedFrameNum = num + m_interpolatedFrame;
		// ��Ԃ̎��Ԃ�����
		m_interpolatedFrame = num;
#else
		m_interpolatedFrameNum = kInterpolatedFrame;
		m_interpolatedFrame = kInterpolatedFrame;
#endif

		m_firstChangeVec = m_nowFront;
		m_lastChangeVec = m_vec;
	}

	// ���`��Ԃ����Ȃ��ꍇ�͕�ԏ������Ȃ�
	if (m_interpolatedFrame < 0) return;
	// ���݂̐��ʕ����̍X�V
	float rate = static_cast<float>(m_interpolatedFrame) / static_cast<float>(m_interpolatedFrameNum);
	m_nowFront = m_lastChangeVec * (1.0f - rate) + m_firstChangeVec * (rate);

	m_nowFront.Normalize();

	m_frontVec = m_nowFront * kSize;
	m_rightVec = m_nowFront.Right() * kSize * 0.5f;
	m_leftVec = m_nowFront.Left() * kSize * 0.5f;

	// ���`��Ԏ��Ԃ̍X�V
	m_interpolatedFrame--;
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