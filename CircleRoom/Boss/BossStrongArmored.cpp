#include <DxLib.h>
#include "Stage/StageBase.h"

#include "Enemy/EnemySplitTwoBound.h"

#include "BossStrongArmored.h"
#include "BossDamageObject.h"

namespace
{
	// FIXME:�m��DxLib�Ń΂���`����Ă��͂������ǖY�ꂽ���玩���Œ�`���Ă���
	constexpr float kPai = 3.1415927f;

	// �J���[
	constexpr int kColor = 0xaaffaa;

	// ��������_���[�W�I�u�W�F�N�g�̐�
	constexpr int kDamageObjectNum = 2;

	// �G������
	constexpr int kCreateNum = 8;
	// �����Ԋu�t���[��
	constexpr int kCreateFrame = static_cast<int>(60 * 2.5);
	// �����O�ҋ@�t���[��
	constexpr int kWaitCreateFrame = 15;
	// ��������Ƃ��̊p�x
	constexpr float kCreateRadian = 45.0f * (kPai / 180.0f);
}

BossStrongArmored::BossStrongArmored(const Size& windowSize, float fieldSize, StageBase* stage) :
	BossArmored(windowSize, fieldSize, stage)
{
	m_name = "BossStrongArmored";
	m_color = kColor;
}

BossStrongArmored::~BossStrongArmored()
{
}

bool BossStrongArmored::OnAttack(bool isDash, const Collision& col)
{
	// �_�b�V�����ł���΂��̏����͂��Ȃ�
	if (isDash) return false;
	bool isHit = false;

	for (const auto& obj : m_objects)
	{
		if (col.IsCollsion(obj->GetRect()))
		{
			m_radian = 0;

			obj->Used();

			isHit = true;

			break;
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
		if (m_objects.size() >= kDamageObjectNum) return isHit;

		m_objects.push_back(std::make_shared<BossDamageObject>(m_windowSize, m_fieldSize));
	}
}

void BossStrongArmored::NormalDraw() const
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), m_color, true);

	// �_���[�W�I�u�W�F�N�g�̕`��
	for (const auto& obj : m_objects)
	{
		obj->Draw();
	}

#ifdef _DEBUG
	// �����蔻��̕`��
	m_col.Draw(0xff0000, false);
#endif
}

void BossStrongArmored::CreateEnemy()
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
			Vec2 vec = m_conversionVec.GetNormalized();
			// ���݂̊p�x�𓾂�
			float radian = atan2f(vec.x, vec.y);

			for (int i = 0; i < kCreateNum; i++)
			{
				std::shared_ptr<EnemySplitTwoBound> split;
				split = std::make_shared<EnemySplitTwoBound>(m_windowSize, m_fieldSize);
				split->Init(m_pos, vec);

				m_stage->GenericEnemy(split);

				// ��]������
				radian += kCreateRadian;

				vec.x = cosf(radian);
				vec.y = sinf(radian);
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
