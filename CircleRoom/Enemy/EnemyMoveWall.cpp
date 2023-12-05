#include "EnemyMoveWall.h"
#include <DxLib.h>
#include "Application.h"

namespace
{
	// 動くスピード
	constexpr float kSpeed = 4.0f;
	// 半径
	constexpr float kRadius = 24.0f;

	// 初めの実体化するまでのフレーム
	constexpr int kApeearFrame = 60;

	// カラー
	constexpr int kColor = 0xffffff;
}

EnemyMoveWall::EnemyMoveWall(const Size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "MoveWall";
}

EnemyMoveWall::~EnemyMoveWall()
{
}

void EnemyMoveWall::Init(Vec2& vec)
{
	// フレームの初期化
	m_frame = 0;

	// 半径の設定
	m_radius = kRadius;

	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

	m_pos = Vec2{centerX + m_fieldSize * -vec.y, centerY};

	m_vec = vec * kSpeed;
}

void EnemyMoveWall::StartUpdate()
{
	m_frame++;
	m_pos += m_vec;

	if (m_frame > kApeearFrame)
	{
		// 変わるときに当たり判定も入れる
		m_rect.SetCenter(m_pos, m_radius);

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyMoveWall::NormalUpdate()
{
	m_pos += m_vec;
	Reflection(false);

	m_rect.SetCenter(m_pos, m_radius);
}

void EnemyMoveWall::StartDraw()
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kApeearFrame);
	int alpha = static_cast<int>(255 * rate);
	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), kColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EnemyMoveWall::NormalDraw()
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), kColor, true);

#ifdef _DEBUG
	// 当たり判定の描画
	m_rect.Draw(0xff0000, false);
#endif
}
