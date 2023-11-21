#include <DxLib.h>
#include "../Application.h"
#include "EnemyNormal.h"

namespace
{
	// 動くスピード
	constexpr float kSpeed = 4.0f;
	// 半径
	constexpr float kRadius = 16.0f;

	// 初めの実体化するまでのフレーム
	constexpr int kApeearFrame = 30;
}

EnemyNormal::EnemyNormal(const Size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
}

EnemyNormal::~EnemyNormal()
{
}

void EnemyNormal::Init(Vec2 pos)
{
	// 引数で渡された位置を初期位置に
	m_pos = pos;
	m_radius = kRadius;
	m_colPos.SetCenter(m_pos, m_radius);

	// フレームの初期化
	m_frame = 0;

	// 撃つ方向をランダムで決める
	float moveX = (GetRand(8) - 4) * 0.25f;
	float moveY = (GetRand(8) - 4) * 0.25f;
	m_vec = Vec2{ moveX, moveY };
	// 正規化してスピードを調整
	m_vec.Normalize();
	m_vec *= kSpeed;
}

void EnemyNormal::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyNormal::NormalUpdate()
{
	m_pos += m_vec;
	Reflection();
}

void EnemyNormal::StartDraw()
{
	int alpha = 255 * (m_frame / kApeearFrame);
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), 0xff0000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EnemyNormal::NormalDraw()
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), 0xff0000, true);
}
