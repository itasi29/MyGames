#include <DxLib.h>
#include <cmath>
#include <memory>

#include "Application.h"
#include "Stage/StageBase.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"

#include "EnemyDivision.h"
#include "EnemySplit.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 4.0f;
	// ���a
	constexpr float kRadius = 43.0f;

	// �J���[
	constexpr int kColor = 0xffffff;

	// ���􂷂鐔
	constexpr int kDivisionNum = 4;
	// ���􂷂�܂ł̃t���[��
	constexpr int kDivisionFrame = 60 * 8;

	// ���􂷂�O�̑ҋ@�t���[��
	constexpr int kDivisionWaitFrame = 60;
	// �����O��]�t���[��
	constexpr int kRotationFrame = 40;

	// �Ō�̉~�̐��̑傫���ƃX�s�[�h
	// MEMO:���̑傫���ƃX�s�[�h�͓����ق������h�����ǂ�
	constexpr int kRipple = 6;
	// �g��̍ő�傫��
	// MEMO:�\�z�����ǂ���ȏア���Ȃ��Ǝv���l
	constexpr int kMaxRippleSize = 1400;
}

EnemyDivision::EnemyDivision(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "Division";
	m_color = kColor;
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Division.png");
}

EnemyDivision::EnemyDivision(const size& windowSize, float fieldSize, StageBase* stage) :
	EnemyBase(windowSize, fieldSize),
	m_stage(stage),
	m_divisionWaitFrame(0),
	m_ripple(kRadius)
{
	m_name = "Division";
	m_color = kColor;
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Division.png");

	m_dUpdateFunc = &EnemyDivision::UsuallyUpdate;
	m_dDrawFunc = &EnemyDivision::UsuallyDraw;

	m_rippleScreen = MakeScreen(m_size.w, m_size.h, true);
}

EnemyDivision::~EnemyDivision()
{
	DeleteGraph(m_rippleScreen);
}

void EnemyDivision::Init(const Vec2& pos, bool isStart)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;

	// �t���[���̏�����
	m_frame = 0;

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

	// ��]������
	m_angle = GetRand(359) * kRad;

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
	m_vec *= kSpeed;
}

void EnemyDivision::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);
		auto& sound = GameManager::GetInstance().GetSound();
		sound->PlaySe(m_createSe->GetHandle());

		EnemyBase::ChangeNormalFunc();

		m_frame = 0;
	}
}

void EnemyDivision::NormalUpdate()
{
	(this->*m_dUpdateFunc)();
}

void EnemyDivision::NormalDraw()
{
	(this->*m_dDrawFunc)();
}

void EnemyDivision::UsuallyUpdate()
{
	m_pos += m_vec;
	m_angle -= kRad;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);

	m_frame++;

	// ��莞�Ԓ������番���O��Ԃɓ���
	if (m_frame > kDivisionFrame)
	{
		m_dUpdateFunc = &EnemyDivision::DivisionUpdate;
	}
}

void EnemyDivision::DivisionUpdate()
{
	m_divisionWaitFrame++;
	if (m_divisionWaitFrame < kRotationFrame)
	{
		m_angle -= (kRad * 20);
	}

	// �ҋ@���Ԃ𒴂����番�􂷂�
	if (m_divisionWaitFrame > kDivisionWaitFrame)
	{
		// �x�N�g����ۑ�����
		Vec2 vec = { cosf(static_cast<float>(m_angle)), sinf(static_cast<float>(m_angle)) };
		for (int i = 0; i < kDivisionNum; i++)
		{
			std::shared_ptr<EnemySplit> split;
			split = std::make_shared<EnemySplit>(m_size, m_fieldSize);
			split->Init(m_pos, vec);

			m_stage->GenericEnemy(split);

			// ��]������
			vec = vec.Right();
		}

		m_dUpdateFunc = &EnemyDivision::EndUpdate;
		m_dDrawFunc = &EnemyDivision::EndDraw;
	}
}

void EnemyDivision::EndUpdate()
{
	m_ripple += kRipple;

	if (m_ripple > kMaxRippleSize)
	{
		m_isExsit = false;
	}
}

void EnemyDivision::UsuallyDraw()
{
	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle,
		m_charImg->GetHandle(), true);

	DrawHitWallEffect();

#ifdef _DEBUG
	// �����蔻��̕`��
	m_col.Draw(0xff0000, false);
#endif
}

void EnemyDivision::EndDraw()
{
	SetDrawScreen(m_rippleScreen);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 16);
	DrawBox(0, 0, m_size.w, m_size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_ripple, 0x0b60b0, false, kRipple);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	int handle = GameManager::GetInstance().GetScene()->GetScreenHandle();
	SetDrawScreen(handle);
	DrawGraph(0, 0, m_rippleScreen, true);
}
