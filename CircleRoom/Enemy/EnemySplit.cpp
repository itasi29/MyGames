#include "EnemySplit.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 6.0f;
	// ���a
	constexpr float kRadius = 20.0f;

	// �J���[
	constexpr int kColor = 0xffffff;
}

EnemySplit::EnemySplit(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "Split";
	m_color = kColor;
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Split.png");
}

EnemySplit::~EnemySplit()
{
}

void EnemySplit::Init(const Vec2& pos, Vec2& vec)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;

	// �ړ��x�N�g���𒲐�
	m_vec = vec;
	m_vec *= kSpeed;

	// ��]������
	m_angle = 0.0;

	// �t���[���̏�����
	m_frame = 0;

	// ���߂��画�������
	m_col.SetCenter(m_pos, m_radius);

	ChangeNormalFunc();
}

void EnemySplit::StartUpdate()
{
}

void EnemySplit::NormalUpdate()
{
	m_pos += m_vec;
	m_angle -= kRad;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);
}
