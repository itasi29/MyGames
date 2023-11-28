#include <DxLib.h>
#include "Application.h"
#include "EnemyNormal.h"

namespace
{
	// 動くスピード
	constexpr float kSpeed = 4.0f;
	// 半径
	constexpr float kRadius = 16.0f;

	// 初めの実体化するまでのフレーム
	constexpr int kApeearFrame = 60;

	// カラー
	constexpr int kColor = 0xffffff;
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

	// フレームの初期化
	m_frame = 0;

	// 撃つ方向をランダムで決める
	float moveX = (GetRand(16) - 8) * 0.125f;
	float moveY = (GetRand(16) - 8) * 0.125f;
	m_vec = Vec2{ moveX, moveY };

	// ゼロベクトルでないなら正規化
	if (m_vec.SqLength() > 0)
	{
		m_vec.Normalize();
	}
	// ゼロベクトルなら方向を真横にする
	else
	{
		m_vec = Vec2{ 1.0f, 0.0f };
	}

	// スピードを調整
	m_vec *= kSpeed;
}

void EnemyNormal::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// 変わるときに当たり判定も入れる
		m_rect.SetCenter(m_pos, m_radius);

		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyNormal::NormalUpdate()
{
	m_pos += m_vec;
	Reflection();

	m_rect.SetCenter(m_pos, m_radius);
}

void EnemyNormal::StartDraw()
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kApeearFrame);
	int alpha = static_cast<int>(255 * rate);
	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), kColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EnemyNormal::NormalDraw()
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), kColor, true);

#ifdef _DEBUG
	// 当たり判定の描画
	m_rect.Draw(0xff0000, false);
#endif

}
