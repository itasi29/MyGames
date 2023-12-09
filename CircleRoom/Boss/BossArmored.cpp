#include <DxLib.h>

#include "BossArmored.h"

namespace
{
	// FIXME:確かDxLibでπが定義されてたはずだけど忘れたから自分で定義しておく
	constexpr float kPai = 3.1415927f;

	// 動くスピード
	constexpr float kSpeed = 3.0f;
	// 揺れ幅
	constexpr float kSwing = 1.5f;

	// 半径
	constexpr float kRadius = 24.0f;

	// カラー
	constexpr int kColor = 0x08ff08;

	// 1フレームごとの角度
	constexpr float kRadian = 1.0f * (kPai / 180.0f);
}

BossArmored::BossArmored(const Size& windowSize, float fieldSize, int maxHp) :
	BossBase(windowSize, fieldSize, maxHp)
{
	m_name = "BossArmored";
	m_color = kColor;
}

BossArmored::~BossArmored()
{
}

void BossArmored::Init(Vec2& pos)
{
	// 引数で渡された位置を初期位置に
	m_pos = pos;
	m_radius = kRadius;

	// ラジアンの初期化
	m_radian = 0;

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
	m_conversionVec = m_vec * kSpeed;
}

void BossArmored::OnAttack(const Collision& rect)
{
	m_radian = 0;

	m_hp--;

	// HPがゼロになったら死亡とする
	if (m_hp <= 0)
	{
		// バーの描画問題でHPを0にしておく
		m_hp = 0;
		m_isExsit = false;
	}
}

void BossArmored::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// 変わるときに当たり判定も入れる
		m_col.SetCenter(m_pos, m_radius);

		BossBase::ChangeNormalFunc();
	}
}

void BossArmored::NormalUpdate()
{
	m_radian += kRadian;

	if (m_radian > 2 * kPai)
	{
		m_radian = 0;
	}

	m_conversionVec.x = m_vec.x * cosf(m_radian);
	m_conversionVec.y = m_vec.y * sinf(m_radian);

	m_conversionVec = m_vec * kSpeed + m_conversionVec * kSwing;

	m_pos += m_conversionVec;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);
}
