#include "EnemyMoveWall.h"
#include <DxLib.h>
#include "Application.h"

namespace
{
	// 動くスピード
	constexpr float kSpeed = 9.0f;
	// 半径
	constexpr float kRadius = 24.0f;

	// カラー
	constexpr int kColor = 0x888888;
}

EnemyMoveWall::EnemyMoveWall(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "MoveWall";
	m_color = kColor;
}

EnemyMoveWall::~EnemyMoveWall()
{
}

void EnemyMoveWall::Init(const Vec2& vec)
{
	// フレームの初期化
	m_frame = 0;

	// 半径の設定
	m_radius = kRadius;

	float centerX = m_size.w * 0.5f;
	float centerY = m_size.h * 0.5f;

	m_pos = Vec2{centerX + m_fieldSize * -vec.y, centerY};

	m_vec = vec * kSpeed;
}

void EnemyMoveWall::StartUpdate()
{
	m_frame++;
	m_pos += m_vec;
	Reflection(0.0f, false);

	if (m_frame > kApeearFrame)
	{
		// 変わるときに当たり判定も入れる
		m_col.SetCenter(m_pos, m_radius);

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyMoveWall::NormalUpdate()
{
	m_pos += m_vec;
	Reflection(0.0f, false);

	m_col.SetCenter(m_pos, m_radius);
}
