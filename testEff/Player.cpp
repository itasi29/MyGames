#include <DxLib.h>

#include "Input.h"
#include "Player.h"

namespace
{
	// �F
	constexpr unsigned int kExsitColor = 0xFFFAE7;
	constexpr unsigned int kDeathColor = 0xD2001A;

	// �����蔻��̔��a�̑傫��
	constexpr float kColRadius = 6.0f;
	// �I�u�W�F�N�g�Ƃ̓����蔻�蔼�a
	constexpr float kObjColRadius = 12.0f;

	// �v���C���[�̑傫��
	constexpr float kSize = 24.0f;
	// �v���C���[�̃X�s�[�h
	constexpr float kSpeed = 4.0f;
	// �v���C���[�̒��S���画����ǂ̂��炢��������
#ifdef false
	constexpr float kColShift = -kSize * 0.12f;
#else
	constexpr float kColShift = kSize * 0.32f;
#endif

	// �ړ��G�t�F�N�g�t���[��
	constexpr int kEffFrame = 20;

	// �_�b�V�����O��
	constexpr int kDashLogNum = 8;
	// �_�b�V�����̃X�s�[�h
	constexpr float kDashSpeed = 12.0f;
	// �_�b�V���\����
	constexpr int kDashFrame = 25;
	// �_�b�V���ҋ@����
	constexpr int kDashWaitFrame = 25;

	// ���`��Ԃ��s���t���[��
	constexpr int kInterpolatedFrame = 50;

	// ���S�G�t�F�N�g1�̏o�Ă���t���[����
	constexpr int kDeathEffectFrame = 3;
	// ���S�G�t�F�N�g�̃t���[��
	constexpr int kDeathFrame = 24 * kDeathEffectFrame;
	// �摜�T�C�Y
	constexpr int kDeathGraphSize = 100;
	// �摜�̍s��
	constexpr int kRow = 6;
	// �摜�̗�
	constexpr int kLine = 6;
}

Player::Player() :
	m_angle(0),
	m_logFrame(kDashLogNum),
	m_dashFrame(0),
	m_dashWaitFrame(0),
	m_isDash(false)
{
	Init();
	m_posLog.resize(kDashLogNum);
	m_dirLog.resize(kDashLogNum);
	m_angleLog.resize(kDashLogNum);

	handle = LoadGraph("_PlayerEff.png");
}

Player::~Player()
{
	DeleteGraph(handle);
}

void Player::Init()
{
	// ����������
	m_effs.clear();
	m_angle = 0;
	m_dashFrame = 0;
	m_dashWaitFrame = 0;
	m_isDash = false;

	// �ʒu�̐ݒ�
	m_pos = {320, 240};

	// �����̐ݒ�
	m_front = Vec2::Up();
	m_dir.front = m_front * kSize;
	m_dir.right = m_front.Right() * kSize * 0.5f;
	m_dir.left = m_front.Left() * kSize * 0.5f;
}

void Player::Update(Input& input)
{
	Move(input);
	Dash(input);
	// �ړ��G�t�F�N�g�̍X�V
	for (auto& eff : m_effs)
	{
		eff.frame++;

		if (eff.frame > kEffFrame)
		{
			eff.isEnd = true;
		}

		eff.pos += eff.vec;

		eff.angle += DX_PI_F / 180.0f;
	}

	m_effs.remove_if(
		[](const auto& eff) {
			return eff.isEnd;
		});
	// ���O�̍X�V
	for (int i = kDashLogNum - 1; i > 0; i--)
	{
		m_posLog[i] = m_posLog[i - 1];
		m_dirLog[i] = m_dirLog[i - 1];
		m_angleLog[i] = m_angleLog[i - 1];
	}
	m_posLog[0] = m_pos;
	m_dirLog[0] = m_dir;
	m_angleLog[0] = m_angle;
	// �ʒu�̍X�V
	m_pos += m_vec;
}

void Player::Draw()
{
	double rate;
	int alpha;

	for (const auto& eff : m_effs)
	{
		rate = 1.0f - eff.frame / static_cast<double>(kEffFrame);
		alpha = static_cast<int>(153 * rate);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraph(eff.pos.x, eff.pos.y, 1.0, eff.angle, handle, true);
	}

	// �_�b�V���������̃��O��`��
	if (m_logFrame < kDashLogNum)
	{
		for (int i = 0; i < kDashLogNum; i++)
		{
			rate = 1.0f - (m_logFrame + i + 1) / static_cast<double>(m_logFrame + kDashLogNum);
			alpha = static_cast<int>(128 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawTriangle(static_cast<int>(m_dirLog[i].front.x + m_posLog[i].x), static_cast<int>(m_dirLog[i].front.y + m_posLog[i].y),
				static_cast<int>(m_dirLog[i].left.x + m_posLog[i].x), static_cast<int>(m_dirLog[i].left.y + m_posLog[i].y),
				static_cast<int>(m_dirLog[i].right.x + m_posLog[i].x), static_cast<int>(m_dirLog[i].right.y + m_posLog[i].y),
				kExsitColor, true);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ���݂̃v���C���[��`��
	DrawTriangleAA(m_dir.front.x + m_pos.x, m_dir.front.y + m_pos.y,
		m_dir.left.x + m_pos.x, m_dir.left.y + m_pos.y,
		m_dir.right.x + m_pos.x, m_dir.right.y + m_pos.y,
		kExsitColor, true);
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
		m_front = m_vec;

		for (int i = 0; i < 1; i++)
		{
			float effX = -m_vec.x * 0.5f + (GetRand(30) * 0.1f - 1.5f);
			float effY = -m_vec.y * 0.5f + (GetRand(30) * 0.1f - 1.5f);

			int randSize = kSize;

			Vec2 pos = m_pos + m_pos.Right().GetNormalized() * (GetRand(randSize) - kSize * 0.5f) * 0.8f;

			m_effs.push_back({ {effX, effY}, pos });
		}

#ifdef false
		m_angle = atan2(m_front.x, -m_front.y);
#else
		m_front = m_vec;
		m_dir.front = m_front * kSize;
		m_dir.right = m_front.Right() * kSize * 0.5f;
		m_dir.left = m_front.Left() * kSize * 0.5f;
#endif
	}

	m_vec *= kSpeed;
}

void Player::Dash(Input& input)
{
	m_logFrame++;

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

		// ���O�����݂̈ʒu�ɂ���
		for (int i = 0; i < kDashLogNum; i++)
		{
			m_posLog[i] = m_pos;
			m_dirLog[i] = m_dir;
			m_angleLog[i] = m_angle;
		}
	}

	// ���݃_�b�V�����łȂ��Ȃ珈���I��
	if (!m_isDash) return;

	// �_�b�V��������
	m_vec = m_front * kDashSpeed;

	// ���O�t���[���̍X�V
	m_logFrame = 0;

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
