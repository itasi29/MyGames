#include <DxLib.h>
#include <cmath>
#include <memory>

#include "Stage/StageBase.h"
#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"

#include "EnemyCreate.h"
#include "EnemyChild.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 3.0f;
	// ���a
	constexpr float kRadius = 43.0f;

	// 1�t���[�����Ƃ̊p�x
	constexpr float kRadian = 2.0f * (DX_PI_F / 180.0f);
	// �c�̗h�ꕝ
	constexpr float kSwingHeight = 2.0f;

	// �����Ԋu
	constexpr int kCreateFrame = 20;
}

EnemyCreate::EnemyCreate(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "Create";
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Create.png");
}

EnemyCreate::EnemyCreate(const size& windowSize, float fieldSize, StageBase* stage) :
	EnemyBase(windowSize, fieldSize),
	m_stage(stage),
	m_radian(0)
{
	m_name = "Create";
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Create.png");
}

EnemyCreate::~EnemyCreate()
{
}

void EnemyCreate::Init(const Vec2& pos, bool isStart)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;

	// ���W�A���̏�����
	m_radian = 0;

	// ��]������
	m_angle = 0.0;

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
}

void EnemyCreate::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);
		auto& sound = GameManager::GetInstance().GetSound();
		sound->PlaySe(m_createSe->GetHandle());

		m_frame = 0;
		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyCreate::NormalUpdate()
{
	m_radian += kRadian;
	if (m_radian > DX_TWO_PI_F)
	{
		m_radian = 0.0f;
	}
	m_angle -= kRad;

	// ���݂̃x�N�g���𐂒������ɃR�T�C���J�[�u�̒l�����炷
	m_conversionVec = m_vec * kSpeed + m_vec.Right() * cosf(m_radian) * kSwingHeight;

	// �o�����x�N�g��������
	m_pos += m_conversionVec;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);

	m_frame++;
	if (m_frame > kCreateFrame)
	{
		m_frame = 0;
		std::shared_ptr<EnemyChild> child;
		child = std::make_shared<EnemyChild>(m_size, m_fieldSize);
		child->Init(m_pos);

		m_stage->GenericEnemy(child);
	}
}
