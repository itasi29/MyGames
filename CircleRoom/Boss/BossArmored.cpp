#include <DxLib.h>

#include "BossArmored.h"

#include "Stage/StageBase.h"
#include "BossDamageObject.h"
#include "Enemy/EnemySplitTwoBound.h"

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

	// 生成するダメージオブジェクトの数
	constexpr int kDamageObjectNum = 2;

	// 敵生成数
	constexpr int kCreateNum = 4;
	// 生成間隔フレーム
	constexpr int kCreateFrame = 60 * 5;
	// 生成前待機フレーム
	constexpr int kWaitCreateFrame = 30;
	// 生成するときの角度
	constexpr float kCreateRadian = 45.0f * (kPai / 180.0f);
}

BossArmored::BossArmored(const Size& windowSize, float fieldSize, int maxHp, StageBase* stage) :
	BossBase(windowSize, fieldSize, maxHp),
	m_stage(stage)
{
	m_name = "BossArmored";
	m_color = kColor;
}

BossArmored::~BossArmored()
{
}

void BossArmored::Init(const Vec2& pos)
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

	m_objects.clear();
	m_objects.push_back(std::make_shared<BossDamageObject>(m_pos));
}

void BossArmored::OnAttack(bool isDash, const Collision& col)
{
	// ダッシュ中であればこの処理はしない
	if (isDash) return;

	for (const auto& obj : m_objects)
	{
		if (col.IsCollsion(obj->GetRect()))
		{
			m_radian = 0;

			m_hp--;

			obj->Used();

			// HPがゼロになったら死亡とする
			if (m_hp <= 0)
			{
				// バーの描画問題でHPを0にしておく
				m_hp = 0;
				m_isExsit = false;

				return;
			}
		}
	}

	// 使用済みのものを削除する
	m_objects.remove_if(
		[](const auto& obj)
		{
			return obj->IsUsed();
		});

	while (1)
	{
		// ダメージオブジェクトの量が規定値以上であれば終了
		if (m_objects.size() >= kDamageObjectNum) return;

		m_objects.push_back(std::make_shared<BossDamageObject>(m_windowSize, m_fieldSize));
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

	CreateEnemy();

	m_pos += m_conversionVec;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);
}

void BossArmored::NormalDraw() const
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), m_color, true);

	// ダメージオブジェクトの描画
	for (const auto& obj : m_objects)
	{
		obj->Draw();
	}

	DrawHpBar();

#ifdef _DEBUG
	// 当たり判定の描画
	m_col.Draw(0xff0000, false);
#endif
}

void BossArmored::CreateEnemy()
{
	// 生成前なら以下if文の処理を行う
	if (m_waitCreateFrame > 0)
	{
		m_waitCreateFrame--;

		// 速度を落とす
		m_conversionVec *= 0.1f;

		if (m_waitCreateFrame <= 0)
		{
			// 生成間隔フレームの初期化
			m_createFrame = kCreateFrame;

			// ベクトルの生成
			Vec2 vec = m_vec;

			// 体力が半分未満なら量倍
			if (m_hp < m_maxHp * 0.5f)
			{
				// 現在の角度を得る
				float radian = atan2f(vec.x, vec.y);

				for (int i = 0; i < kCreateNum * 2; i++)
				{
					std::shared_ptr<EnemySplitTwoBound> split;
					split = std::make_shared<EnemySplitTwoBound>(m_windowSize, m_fieldSize);
					split->Init(m_pos, vec);

					m_stage->GenericEnemy(split);

					radian += kCreateRadian;

					vec.x = cosf(radian);
					vec.y = sinf(radian);
				}
			}
			else
			{
				for (int i = 0; i < kCreateNum; i++)
				{
					std::shared_ptr<EnemySplitTwoBound> split;
					split = std::make_shared<EnemySplitTwoBound>(m_windowSize, m_fieldSize);
					split->Init(m_pos, vec);

					m_stage->GenericEnemy(split);

					// 回転させる
					vec = vec.Right();
				}
			}
		}

		return;
	}

	m_createFrame--;

	if (m_createFrame < 0)
	{
		// 生成前待機フレームの初期化
		m_waitCreateFrame = kWaitCreateFrame;
	}
}
