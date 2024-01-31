#include <DxLib.h>
#include <cmath>
#include <memory>

#include "Stage/StageBase.h"
#include "GameManager.h"
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
	m_isDivisionWait(false),
	m_divisionWaitFrame(0)
{
	m_name = "Division";
	m_color = kColor;
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Division.png");
}

EnemyDivision::~EnemyDivision()
{
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
	// �����ҋ@��ԂȂ�ȉ��̏�����
	if (m_isDivisionWait)
	{
		m_divisionWaitFrame++;
		if (m_divisionWaitFrame < kRotationFrame)
		{
			m_angle -= (kRad * 20);
		}

		// �ҋ@���Ԃ𒴂����番�􂷂�
		if (m_divisionWaitFrame > kDivisionWaitFrame)
		{
			m_isExsit = false;

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
		}

		return;
	}

	m_pos += m_vec;
	m_angle -= kRad;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);

	m_frame++;

	// ��莞�Ԓ������番���O��Ԃɓ���
	if (m_frame > kDivisionFrame)
	{
		m_isDivisionWait = true;
	}
}
