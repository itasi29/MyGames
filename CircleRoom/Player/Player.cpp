#include <DxLib.h>
#include "Application.h"
#include "Common/Input.h"
#include "Stage/StageManager.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

#include "Player.h"


namespace
{
	// �����蔻��̔��a�̑傫��
	constexpr float kColRadius = 10.0f;

	// �v���C���[�̑傫��
	constexpr float kSize = 24.0f;
	// �v���C���[�̃X�s�[�h
	constexpr float kSpeed = 4.0f;
	// �v���C���[�̒��S���画����ǂ̂��炢��������
	constexpr float kColShift = -kSize * 0.12f;

	// �ړ��G�t�F�N�g�t���[��
	constexpr int kEffFrame = 10;

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

Player::Player(const size& windowSize, float fieldSize) :
	m_size(windowSize),
	m_fieldSize(fieldSize),
	m_angle(0),
	m_logFrame(kDashLogNum),
	m_dashFrame(0),
	m_dashWaitFrame(0),
	m_isDash(false),
	m_isExsit(false),
	m_deathFrame(0),
	m_isDeathEffect(false)
{
	Init();
	m_isExsit = false;
	m_posLog.resize(kDashLogNum);
	m_angleLog.resize(kDashLogNum);

	auto& mgr = GameManager::GetInstance();
	m_bloodImg = mgr.GetFile()->LoadGraphic(L"Player/blood.png");
	m_charImg = mgr.GetFile()->LoadGraphic(L"Player/Player.png");
	m_charDeathImg = mgr.GetFile()->LoadGraphic(L"Player/PlayerDeath.png");
	m_charEffImg = mgr.GetFile()->LoadGraphic(L"Player/PlayerEff.png");
}

Player::~Player()
{
}

void Player::Init()
{
	// ����������
	m_effs.clear();
	m_angle = 0;
	m_dashFrame = 0;
	m_dashWaitFrame = 0;
	m_isDash = false;
	m_isExsit = true;
	m_isDeathEffect = false;

	// �ʒu�̐ݒ�
	m_pos = Vec2{ m_size.w / 2.0f, m_size.h - m_fieldSize };

	// �����̐ݒ�
	m_front = Vec2::Up();

	// �����蔻��̍X�V
	m_col.SetCenter(m_pos, kColRadius, m_front.x * kColShift, m_front.y * kColShift);
}

void Player::Update(Input& input, Ability ability)
{
	// ���S�G�t�F�N�g
	if (m_isDeathEffect)
	{
		m_deathFrame++;

		if (m_deathFrame > kDeathFrame)
		{
			m_isDeathEffect = false;
		}
	}

	// ����ł���Ώ��������Ȃ�
	if (!m_isExsit) return;

	Move(input);
	// �A�r���e�B����
	switch (ability)
	{
		// �A�r���e�B�Ȃ�
	case kNone:
	default:
		break;

		// �_�b�V��
	case kDash:
		Dash(input);
		break;
	}
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
		m_angleLog[i] = m_angleLog[i - 1];
	}
	m_posLog[0] = m_pos;
	m_angleLog[0] = m_angle;
	// �ʒu�̍X�V
	m_pos += m_vec;

	InRange();

	// �����蔻��̍X�V
	m_col.SetCenter(m_pos, kColRadius, m_front.x * kColShift, m_front.y * kColShift);
}

void Player::Draw()
{
	if (m_isExsit)
	{
		float rate;
		int alpha;

		// �ړ����̃G�t�F�N�g��`��
		for (const auto& eff : m_effs)
		{
			rate = 1.0f - (eff.frame / static_cast<float>(kEffFrame));
			alpha = 255 * rate;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawRotaGraph(static_cast<int>(eff.pos.x), static_cast<int>(eff.pos.y), rate, eff.angle, m_charEffImg->GetHandle(), true);
		}

		// �_�b�V���������̃��O��`��
		if (m_logFrame < kDashLogNum)
		{
			for (int i = 0; i < kDashLogNum; i++)
			{
				rate = 1.0f - (m_logFrame + i + 1) / static_cast<float>(m_logFrame + kDashLogNum);
				alpha = 128 * rate;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
				DrawRotaGraph(static_cast<int>(m_posLog[i].x), static_cast<int>(m_posLog[i].y), 1.0, m_angleLog[i], m_charImg->GetHandle(), true);
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle, m_charImg->GetHandle(), true);
	}
	else
	{
		DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle, m_charDeathImg->GetHandle(), true);
	}

	// ���S���̃G�t�F�N�g
	if (m_isDeathEffect)
	{
		int x = m_pos.x - kDeathGraphSize * 0.5f;
		int y = m_pos.y - kDeathGraphSize * 0.5f;
		int index = m_deathFrame / kDeathEffectFrame;
		int srcX = kDeathGraphSize * (index % kRow);
		int srcY = kDeathGraphSize * (index / kLine);

		DrawRectGraph(x, y, srcX, srcY, kDeathGraphSize, kDeathGraphSize, m_bloodImg->GetHandle(), true);
	}

#ifdef _DEBUG
	// �����Ă��Ȃ��ꍇ�����蔻��̕`��
	if (!m_isDash)
	{
		m_col.Draw(0xff0000, false);
	}
#endif
}

void Player::Death()
{
	m_isExsit = false;

	m_deathFrame = 0;
	m_isDeathEffect = true;
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

		float effX = -m_vec.x + (GetRand(10) * 0.1f - 0.5f);
		float effY = -m_vec.y + (GetRand(10) * 0.1f - 0.5f);
		m_effs.push_back({ {effX, effY}, m_pos });

		m_angle = atan2(m_front.x, -m_front.y);
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
			m_angleLog[i] = m_angle;
		}

		GameManager::GetInstance().UpdateDashCount();
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

void Player::InRange()
{
	float centerX = m_size.w *0.5f;
	float centerY = m_size.h * 0.5f;

	// ������
	// ���݂̈ʒu���琳�ʕ������̌����ɒ��S�����炵(�����蔻��̈ʒu�Ɏ����Ă���)
	// �����蔻��̔��a�����炵���ʒu����O�ňʒu�����炷
	if (m_pos.x + m_front.x * kColShift - kColRadius < centerX - m_fieldSize)
	{
		// ��O�̒[���瓖���蔻��̈ʒu�����炵
		// ���a���E�ɂ��炷
		m_pos.x = centerX - m_fieldSize - m_front.x * kColShift + kColRadius;
	}
	// �E����
	if (m_pos.x + m_front.x * kColShift + kColRadius > centerX + m_fieldSize)
	{
		m_pos.x = centerX + m_fieldSize - m_front.x * kColShift - kColRadius;
	}
	// �㏈��
	if (m_pos.y + m_front.y * kColShift - kColRadius < centerY - m_fieldSize)
	{
		m_pos.y = centerY - m_fieldSize - m_front.y * kColShift + kColRadius;
	}
	// ������
	if (m_pos.y + m_front.y * kColShift + kColRadius> centerY + m_fieldSize)
	{
		m_pos.y = centerY + m_fieldSize - m_front.y * kColShift - kColRadius;
	}
}
