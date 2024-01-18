#include "EnemySplit.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

namespace
{
	// �����X�s�[�h
	constexpr float kSpeed = 6.0f;
	// ���a
	constexpr float kRadius = 12.0f;

	// �J���[
	constexpr int kColor = 0xffffff;
}

EnemySplit::EnemySplit(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "Split";
	m_color = kColor;

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
	Reflection();

	m_col.SetCenter(m_pos, m_radius);
}
