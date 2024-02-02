#include <DxLib.h>
#include <cmath>

#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/ImageFile.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"

#include "BossArmored.h"

#include "Stage/StageBase.h"
#include "BossDamageObject.h"
#include "Enemy/EnemySplitTwoBound.h"

namespace
{
	// �ő�HP
#ifdef _DEBUG
	constexpr int kMaxHp = 40;
#else
	constexpr int kMaxHp = 40;
#endif

	// FIXME:�m��DxLib�Ń΂���`����Ă��͂������ǖY�ꂽ���玩���Œ�`���Ă���
	constexpr float kPai = 3.1415927f;

	// �����X�s�[�h
	constexpr float kSpeed = 3.0f;
	// �h�ꕝ
	constexpr float kSwing = 1.5f;

	// ���a
	constexpr float kRadius = 80.0f;

	// �J���[
	constexpr int kColor = 0x08ff08;

	// 1�t���[�����Ƃ̊p�x
	constexpr float kRadian = 1.0f * (kPai / 180.0f);

	// ��������_���[�W�I�u�W�F�N�g�̐�
	constexpr int kDamageObjectNum = 1;

	// �G������
	constexpr int kCreateNum = 4;
	// �����Ԋu�t���[��
	constexpr int kCreateFrame = static_cast<int>(60 * 4.5);
	// �����O�ҋ@�t���[��
	constexpr int kWaitCreateFrame = 30;
	// ��������Ƃ��̊p�x
	constexpr float kCreateRadian = 45.0f * (kPai / 180.0f);

	// �_���[�W���󂯂��ۉ�ʂ�h�炷����
	constexpr int kShakeFrame = 5;
	// �h�炷�T�C�Y
	constexpr int kShakeSize = 20;
}

BossArmored::BossArmored(const size& windowSize, float fieldSize) :
	BossBase(windowSize, fieldSize, kMaxHp)
{
	m_name = "BossArmored";
	m_color = kColor;
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/BossArmored.png");
}

BossArmored::BossArmored(const size& windowSize, float fieldSize, StageBase* stage) :
	BossBase(windowSize, fieldSize, kMaxHp),
	m_stage(stage)
{
	m_name = "BossArmored";
	m_color = kColor;
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/BossArmored.png");
}

BossArmored::~BossArmored()
{
}

void BossArmored::Init(const Vec2& pos, bool isStart)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;

	// hp�̏�����
	m_hp = m_maxHp;
	// �o�[�̏�����
	m_hpWidth = kHpBarWidth;
	m_hpDownWidth = kHpBarWidth;

	// ���W�A���̏�����
	m_radian = 0;

	// ��]������
	m_angle = 0.0;

	// �t���[���̏�����
	m_frame = 0;
	m_createFrame = kCreateFrame;
	m_waitCreateFrame = kWaitCreateFrame;

	// �������������_���Ō��߂�
	do
	{
		float moveX = (GetRand(16) - 8) * 0.125f;
		float moveY = (GetRand(16) - 8) * 0.125f;

		if (isStart)
		{
			double angle = atan2(moveX, -moveY);

			if (angle >= (DX_PI / 180 * 135)) continue;
			if (angle <= -(DX_PI / 180 * 135)) continue;
		}

		m_vec = Vec2{ moveX, moveY };
	} while (false);

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
	m_objects.push_back(std::make_shared<BossDamageObject>(m_pos, this));
}

bool BossArmored::OnAttack(bool isDash, const Collision& col)
{
	// �_�b�V�����ł���΂��̏����͂��Ȃ�
	if (isDash) return false;
	bool isHit = false;

	int objAdd = 0;

	for (const auto& obj : m_objects)
	{
		if (obj->IsPickUp())
		{
#if false
			if (obj->IsEnd())
			{
				auto& sound = GameManager::GetInstance().GetSound();
				sound->PlaySe(m_damageSe->GetHandle());

				m_onDamagetFrame = kOnDamageFrame;
				m_drawOnDamagetX = static_cast<int>(m_pos.x);
				m_drawOnDamagetY = static_cast<int>(m_pos.y);

				m_radian = 0;

				isHit = true;

				m_hp--;
				m_hpWidth = static_cast<int>(kHpBarWidth * (m_hp / static_cast<float>(m_maxHp)));

				// HP���[���ɂȂ����玀�S�Ƃ���
				if (m_hp <= 0)
				{
					// �o�[�̕`�����HP��0�ɂ��Ă���
					m_hp = 0;
					m_isExsit = false;
					OnDeath();

					return isHit;
				}

				GameManager& mgr = GameManager::GetInstance();
				mgr.GetScene()->ShakeScreen(kShakeFrame, kShakeSize);
				HitStop();

			}
#endif
			continue;
		}

		if (col.IsCollsion(obj->GetRect()))
		{
			obj->OnUse();

			objAdd++;

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

void BossArmored::OnDamage()
{
	auto& sound = GameManager::GetInstance().GetSound();
	sound->PlaySe(m_damageSe->GetHandle());

	m_stage->UpTime();

	m_onDamagetFrame = kOnDamageFrame;
	m_drawOnDamagetX = static_cast<int>(m_pos.x);
	m_drawOnDamagetY = static_cast<int>(m_pos.y);

	m_radian = 0;

	m_hp--;
	m_hpWidth = static_cast<int>(kHpBarWidth * (m_hp / static_cast<float>(m_maxHp)));

	// HP���[���ɂȂ����玀�S�Ƃ���
	if (m_hp <= 0)
	{
		// �o�[�̕`�����HP��0�ɂ��Ă���
		m_hp = 0;
		m_isExsit = false;
		OnDeath();

		return;
	}

	GameManager& mgr = GameManager::GetInstance();
	mgr.GetScene()->ShakeScreen(kShakeFrame, kShakeSize);
	//HitStop();
	return;
}

void BossArmored::DeleteDamageObjects()
{
	// �_���[�W�I�u�W�F�N�g�̍폜
	m_objects.clear();
}

void BossArmored::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);
		auto& sound = GameManager::GetInstance().GetSound();
		sound->PlaySe(m_createSe->GetHandle());

		BossBase::ChangeNormalFunc();

#if false
		m_updateFunc = &BossBase::NormalUpdate;
		m_drawFunc = &BossBase::NormalDraw;
#endif
	}
}

void BossArmored::NormalUpdate()
{
	m_onDamagetFrame--;

	m_radian += kRadian;

	if (m_radian > 2 * kPai)
	{
		m_radian = 0;
	}
	m_angle -= kRad;

	m_conversionVec.x = m_vec.x * cosf(m_radian);
	m_conversionVec.y = m_vec.y * sinf(m_radian);

	m_conversionVec = m_vec * kSpeed + m_conversionVec * kSwing;

	CreateEnemy();

	m_pos += m_conversionVec;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);

	for (const auto& obj : m_objects)
	{
		obj->Update();
	}

	if (m_hpDownWidth > m_hpWidth)
	{
		m_hpDownWidth--;
	}
}

void BossArmored::NormalDraw() const
{
	// �e�̕`��
	DrawRotaGraph(static_cast<int>(m_pos.x + 10), static_cast<int>(m_pos.y + 10), 1.0, m_angle,
		m_shadow->GetHandle(), true);

	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle,
		m_charImg->GetHandle(), true);

	// �_���[�W�I�u�W�F�N�g�̕`��
	for (const auto& obj : m_objects)
	{
		obj->Draw();
	}

	DrawDamageEffect();

	DrawHitWallEffect();

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
			Vec2 vec = m_conversionVec.GetNormalized();

			// �̗͂����������Ȃ�ʔ{
			if (m_hp < m_maxHp * 0.5f)
			{
				// ���݂̊p�x�𓾂�
				float radian = atan2f(vec.x, vec.y);

				for (int i = 0; i < kCreateNum * 2 + 1; i++)
				{
					std::shared_ptr<EnemySplitTwoBound> split;
					split = std::make_shared<EnemySplitTwoBound>(m_size, m_fieldSize);
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
					split = std::make_shared<EnemySplitTwoBound>(m_size, m_fieldSize);
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
