#include <DxLib.h>
#include <cmath>
#include <memory>

#include "Stage/StageBase.h"

#include "EnemyCreate.h"
#include "EnemyChild.h"

namespace
{
	// FIXME:確かDxLibでπが定義されてたはずだけど忘れたから自分で定義しておく
	constexpr float kPai = 3.1415926535f;

	// 動くスピード
	constexpr float kSpeed = 5.0f;
	// 半径
	constexpr float kRadius = 24.0f;

	// カラー
	constexpr int kColor = 0xffffff;

	// 1フレームごとの角度
	constexpr float kRadian = 5.0f * (kPai / 180.0f);
	// 縦の揺れ幅
	constexpr float kSwingHeight = 2.0f;

	// 生成間隔
	constexpr int kCreateFrame = 15;
}

EnemyCreate::EnemyCreate(const Size& windowSize, float fieldSize, StageBase* stage) :
	EnemyBase(windowSize, fieldSize),
	m_stage(stage),
	m_radian(0)
{
	m_name = "Create";
	m_color = kColor;
}

EnemyCreate::~EnemyCreate()
{
}

void EnemyCreate::Init(const Vec2& pos)
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

void EnemyCreate::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// 変わるときに当たり判定も入れる
		m_col.SetCenter(m_pos, m_radius);

		m_frame = 0;
		EnemyBase::ChangeNormalFunc();
	}
}

void EnemyCreate::NormalUpdate()
{
	m_radian += kRadian;
	if (m_radian > 2 * kPai)
	{
		m_radian = 0.0f;
	}

	// 現在のベクトルを垂直方向にコサインカーブの値分ずらす
	m_conversionVec = m_vec * kSpeed + m_vec.Right() * cosf(m_radian) * kSwingHeight;

	// 出したベクトルを入れる
	m_pos += m_conversionVec;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);

	m_frame++;
	if (m_frame > kCreateFrame)
	{
		m_frame = 0;
		std::shared_ptr<EnemyChild> child;
		child = std::make_shared<EnemyChild>(m_windowSize, m_fieldSize);
		child->Init(m_pos);

		m_stage->GenericEnemy(child);
	}
}
