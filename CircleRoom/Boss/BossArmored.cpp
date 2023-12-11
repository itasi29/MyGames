#include <DxLib.h>

#include "BossArmored.h"

#include "Stage/StageBase.h"
#include "BossDamageObject.h"
#include "Enemy/EnemySplitTwoBound.h"

namespace
{
	// FIXME:�m��DxLib�Ń΂���`����Ă��͂������ǖY�ꂽ���玩���Œ�`���Ă���
	constexpr float kPai = 3.1415927f;

	// �����X�s�[�h
	constexpr float kSpeed = 3.0f;
	// �h�ꕝ
	constexpr float kSwing = 1.5f;

	// ���a
	constexpr float kRadius = 24.0f;

	// �J���[
	constexpr int kColor = 0x08ff08;

	// 1�t���[�����Ƃ̊p�x
	constexpr float kRadian = 1.0f * (kPai / 180.0f);

	// ��������_���[�W�I�u�W�F�N�g�̐�
	constexpr int kDamageObjectNum = 2;

	// �G������
	constexpr int kCreateNum = 4;
	// �����Ԋu�t���[��
	constexpr int kCreateFrame = 60 * 5;
	// �����O�ҋ@�t���[��
	constexpr int kWaitCreateFrame = 30;
	// ��������Ƃ��̊p�x
	constexpr float kCreateRadian = 45.0f * (kPai / 180.0f);
}

BossArmored::BossArmored(const Size& windowSize, float fieldSize, int maxHp, StageBase* stage) :
	BossBase(windowSize, fieldSize, maxHp),
	m_stage(stage)
{
	m_name = "BossArmored";
	m_color = kColor;
}

BossArmored::~BossArmored()
{
}

void BossArmored::Init(const Vec2& pos)
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

	m_objects.clear();
	m_objects.push_back(std::make_shared<BossDamageObject>(m_pos));
}

void BossArmored::OnAttack(bool isDash, const Collision& col)
{
	// �_�b�V�����ł���΂��̏����͂��Ȃ�
	if (isDash) return;

	for (const auto& obj : m_objects)
	{
		if (col.IsCollsion(obj->GetRect()))
		{
			m_radian = 0;

			m_hp--;

			obj->Used();

			// HP���[���ɂȂ����玀�S�Ƃ���
			if (m_hp <= 0)
			{
				// �o�[�̕`�����HP��0�ɂ��Ă���
				m_hp = 0;
				m_isExsit = false;

				return;
			}
		}
	}

	// �g�p�ς݂̂��̂��폜����
	m_objects.remove_if(
		[](const auto& obj)
		{
			return obj->IsUsed();
		});

	while (1)
	{
		// �_���[�W�I�u�W�F�N�g�̗ʂ��K��l�ȏ�ł���ΏI��
		if (m_objects.size() >= kDamageObjectNum) return;

		m_objects.push_back(std::make_shared<BossDamageObject>(m_windowSize, m_fieldSize));
	}
}

void BossArmored::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);

		BossBase::ChangeNormalFunc();
	}
}

void BossArmored::NormalUpdate()
{
	m_radian += kRadian;

	if (m_radian > 2 * kPai)
	{
		m_radian = 0;
	}

	m_conversionVec.x = m_vec.x * cosf(m_radian);
	m_conversionVec.y = m_vec.y * sinf(m_radian);

	m_conversionVec = m_vec * kSpeed + m_conversionVec * kSwing;

	CreateEnemy();

	m_pos += m_conversionVec;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);
}

void BossArmored::NormalDraw() const
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), m_color, true);

	// �_���[�W�I�u�W�F�N�g�̕`��
	for (const auto& obj : m_objects)
	{
		obj->Draw();
	}

	DrawHpBar();

#ifdef _DEBUG
	// �����蔻��̕`��
	m_col.Draw(0xff0000, false);
#endif
}

void BossArmored::CreateEnemy()
{
	// �����O�Ȃ�ȉ�if���̏������s��
	if (m_waitCreateFrame > 0)
	{
		m_waitCreateFrame--;

		// ���x�𗎂Ƃ�
		m_conversionVec *= 0.1f;

		if (m_waitCreateFrame <= 0)
		{
			// �����Ԋu�t���[���̏�����
			m_createFrame = kCreateFrame;

			// �x�N�g���̐���
			Vec2 vec = m_vec;

			// �̗͂����������Ȃ�ʔ{
			if (m_hp < m_maxHp * 0.5f)
			{
				// ���݂̊p�x�𓾂�
				float radian = atan2f(vec.x, vec.y);

				for (int i = 0; i < kCreateNum * 2; i++)
				{
					std::shared_ptr<EnemySplitTwoBound> split;
					split = std::make_shared<EnemySplitTwoBound>(m_windowSize, m_fieldSize);
					split->Init(m_pos, vec);

					m_stage->GenericEnemy(split);

					radian += kCreateRadian;

					vec.x = cosf(radian);
					vec.y = sinf(radian);
				}
			}
			else
			{
				for (int i = 0; i < kCreateNum; i++)
				{
					std::shared_ptr<EnemySplitTwoBound> split;
					split = std::make_shared<EnemySplitTwoBound>(m_windowSize, m_fieldSize);
					split->Init(m_pos, vec);

					m_stage->GenericEnemy(split);

					// ��]������
					vec = vec.Right();
				}
			}
		}

		return;
	}

	m_createFrame--;

	if (m_createFrame < 0)
	{
		// �����O�ҋ@�t���[���̏�����
		m_waitCreateFrame = kWaitCreateFrame;
	}
}
