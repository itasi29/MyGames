#include <DxLib.h>
#include <cmath>
#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"

#include "EnemyLarge.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 2.5f;
	// ���a
	constexpr float kRadius = 48.0f;

	// �J���[
	constexpr int kColor = 0xffffff;
}

EnemyLarge::EnemyLarge(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "Large";
	m_color = kColor;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Large.png");
}

EnemyLarge::~EnemyLarge()
{
}

void EnemyLarge::Init(const Vec2& pos)
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

		double angle = atan2(moveX, -moveY);

		if (angle >= (DX_PI / 180 * 135)) continue;
		if (angle <= -(DX_PI / 180 * 135)) continue;


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
	m_vec *= kSpeed;
}

void EnemyLarge::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// �ς��Ƃ��ɓ����蔻��������
		m_col.SetCenter(m_pos, m_radius);
		auto& sound = GameManager::GetInstance().GetSound();
		sound->PlaySe(m_createSe->GetHandle());

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyLarge::NormalUpdate()
{
	m_pos += m_vec;
	Reflection(0.8f);

	m_col.SetCenter(m_pos, m_radius);
}
