#include <DxLib.h>

#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "Stage/StageBase.h"
#include "FileSystem/ImageFile.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"

#include "Enemy/EnemySplitTwoBound.h"

#include "BossStrongArmored.h"
#include "BossDamageObject.h"

namespace
{
	// �J���[
	constexpr int kColor = 0xaaffaa;

	// ��������_���[�W�I�u�W�F�N�g�̐�
	constexpr int kDamageObjectNum = 1;

	// �G������
	constexpr int kCreateNum = 9;
	// �����Ԋu�t���[��
	constexpr int kCreateFrame = static_cast<int>(60 * 2.5);
	// �����O�ҋ@�t���[��
	constexpr int kWaitCreateFrame = 15;
	// ��������Ƃ��̊p�x
	constexpr float kCreateRadian = 45.0f * (DX_PI_F / 180.0f);

	// �_���[�W���󂯂��ۂ̃t���[��
	constexpr int kOnDamageFrame = 33;
	// �_���[�W���󂯂��ۉ�ʂ�h�炷����
	constexpr int kShakeFrame = 5;
	// �h�炷�T�C�Y
	constexpr int kShakeSize = 20;


	// �_���[�W�G�t�F�N�g�̉摜�T�C�Y
	constexpr int kDamageGraphSize = 32;
	// �_���[�W�G�t�F�N�g�T�C�Y
	constexpr double kDamageSize = 2.0;
	// �_���[�W�G�t�F�N�g�̉摜�̏c�؂���ʒu
	constexpr int kSrcY = 8 * 64;
}

BossStrongArmored::BossStrongArmored(const size& windowSize, float fieldSize) :
	BossArmored(windowSize, fieldSize)
{
	m_name = "BossStrongArmored";
	m_color = kColor;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_char[0] = mgr->LoadGraphic(L"Enemy/BossStrongArmoredOutside.png");
	m_char[1] = mgr->LoadGraphic(L"Enemy/BossStrongArmoredMiddle.png");
	m_char[2] = mgr->LoadGraphic(L"Enemy/BossStrongArmoredCenter.png");
}

BossStrongArmored::BossStrongArmored(const size& windowSize, float fieldSize, StageBase* stage) :
	BossArmored(windowSize, fieldSize, stage)
{
	m_name = "BossStrongArmored";
	m_color = kColor;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_char[0] = mgr->LoadGraphic(L"Enemy/BossStrongArmoredOutside.png");
	m_char[1] = mgr->LoadGraphic(L"Enemy/BossStrongArmoredMiddle.png");
	m_char[2] = mgr->LoadGraphic(L"Enemy/BossStrongArmoredCenter.png");
}

BossStrongArmored::~BossStrongArmored()
{
}

bool BossStrongArmored::OnAttack(bool isDash, const Collision& col)
{
	// �_�b�V�����ł���΂��̏����͂��Ȃ�
	if (isDash) return false;
	bool isHit = false;

	int objAdd = 0;

	for (const auto& obj : m_objects)
	{
		if (obj->IsPickUp())	continue;

		if (col.IsCollsion(obj->GetRect()))
		{
			
			objAdd++;

			obj->OnUse();

			break;
		}
	}

	// �g�p�ς݂̂��̂��폜����
	m_objects.remove_if(
		[](const auto& obj)
		{
			return obj->IsEnd();
		});

	for (int i = 0; i < objAdd; i++)
	{
		m_objects.push_back(std::make_shared<BossDamageObject>(m_size, m_fieldSize, this));
	}

	return isHit;
}

void BossStrongArmored::OnDamage()
{
	auto& sound = GameManager::GetInstance().GetSound();
	sound->PlaySe(m_damageSe->GetHandle());

	m_stage->UpTime();

	m_radian = 0;

	GameManager& mgr = GameManager::GetInstance();
	mgr.GetScene()->ShakeScreen(kShakeFrame, kShakeSize);
//	HitStop();

	return;
}

void BossStrongArmored::NormalDraw() const
{
	// �e�̕`��
	DrawRotaGraph(static_cast<int>(m_pos.x + 10), static_cast<int>(m_pos.y + 10), 1.0, m_angle[0],
		m_shadow->GetHandle(), true);

	for (int i = 0; i < kGraphNum; i++)
	{
		DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle[i],
			m_char[i]->GetHandle(), true);
	}

	DrawHitWallEffect();

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
				split = std::make_shared<EnemySplitTwoBound>(m_size, m_fieldSize);
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
