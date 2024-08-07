#include <DxLib.h>
#include "Application.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

#include "EnemyMoveWall.h"

namespace
{
	// 動くスピード
	constexpr float kSpeed = 9.0f;
	// 半径
	constexpr float kRadius = 43.0f;
}

EnemyMoveWall::EnemyMoveWall(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "MoveWall";
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Wall.png");
}

EnemyMoveWall::~EnemyMoveWall()
{
}

void EnemyMoveWall::Init(const Vec2& vec, bool isStart)
{
	// フレームの初期化
	m_frame = 0;

	// 回転初期化
	m_angle = 0.0;

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
	m_angle -= kRad;
	Reflection(0.0f, false);

	m_col.SetCenter(m_pos, m_radius);
}
