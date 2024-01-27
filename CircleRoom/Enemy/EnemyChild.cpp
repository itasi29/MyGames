#include "EnemyChild.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

namespace
{
	// ���a
	constexpr float kRadius = 8.0f;

	// �J���[
	constexpr int kColor = 0xf0f008;

	// ������܂ł̃t���[��
	constexpr int kDeleteFrame = 60 * 2;
}

EnemyChild::EnemyChild(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "Child";
	m_color = kColor;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Child.png");
}

EnemyChild::~EnemyChild()
{
}

void EnemyChild::Init(const Vec2& pos, bool isStart)
{
	// �����œn���ꂽ�ʒu�������ʒu��
	m_pos = pos;
	m_radius = kRadius;

	// �t���[���̏�����
	m_frame = 0;

	// ���߂��画�������
	m_col.SetCenter(m_pos, m_radius);

	// �X�V�ꏊ�����ύX������
	EnemyBase::ChangeNormalFunc();
}

void EnemyChild::StartUpdate()
{
	// �����Ȃ�
}

void EnemyChild::NormalUpdate()
{
	m_frame++;

	if (m_frame > kDeleteFrame)
	{
		m_isExsit = false;
	}
}
