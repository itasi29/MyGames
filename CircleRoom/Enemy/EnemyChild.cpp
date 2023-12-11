#include "EnemyChild.h"

namespace
{
	// 半径
	constexpr float kRadius = 8.0f;

	// カラー
	constexpr int kColor = 0xf0f008;

	// 消えるまでのフレーム
	constexpr int kDeleteFrame = 60 * 2;
}

EnemyChild::EnemyChild(const Size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "Child";
	m_color = kColor;
}

EnemyChild::~EnemyChild()
{
}

void EnemyChild::Init(const Vec2& pos)
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
