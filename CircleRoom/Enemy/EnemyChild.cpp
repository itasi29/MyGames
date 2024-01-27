#include "EnemyChild.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

namespace
{
	// 半径
	constexpr float kRadius = 8.0f;

	// カラー
	constexpr int kColor = 0xf0f008;

	// 消えるまでのフレーム
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
	// 引数で渡された位置を初期位置に
	m_pos = pos;
	m_radius = kRadius;

	// フレームの初期化
	m_frame = 0;

	// 初めから判定を持つ
	m_col.SetCenter(m_pos, m_radius);

	// 更新場所等も変更させる
	EnemyBase::ChangeNormalFunc();
}

void EnemyChild::StartUpdate()
{
	// 何もなし
}

void EnemyChild::NormalUpdate()
{
	m_frame++;

	if (m_frame > kDeleteFrame)
	{
		m_isExsit = false;
	}
}
