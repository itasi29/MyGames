#include <DxLib.h>
#include "Stage/StageBase.h"

#include "Enemy/EnemySplitTwoBound.h"

#include "BossStrongArmored.h"
#include "BossDamageObject.h"

namespace
{
	// FIXME:確かDxLibでπが定義されてたはずだけど忘れたから自分で定義しておく
	constexpr float kPai = 3.1415927f;

	// カラー
	constexpr int kColor = 0xaaffaa;

	// 生成するダメージオブジェクトの数
	constexpr int kDamageObjectNum = 2;

	// 敵生成数
	constexpr int kCreateNum = 8;
	// 生成間隔フレーム
	constexpr int kCreateFrame = static_cast<int>(60 * 2.5);
	// 生成前待機フレーム
	constexpr int kWaitCreateFrame = 15;
	// 生成するときの角度
	constexpr float kCreateRadian = 45.0f * (kPai / 180.0f);
}

BossStrongArmored::BossStrongArmored(const Size& windowSize, float fieldSize, StageBase* stage) :
	BossArmored(windowSize, fieldSize, stage)
{
	m_name = "BossStrongArmored";
	m_color = kColor;
}

BossStrongArmored::~BossStrongArmored()
{
}

bool BossStrongArmored::OnAttack(bool isDash, const Collision& col)
{
	// ダッシュ中であればこの処理はしない
	if (isDash) return false;
	bool isHit = false;

	for (const auto& obj : m_objects)
	{
		if (col.IsCollsion(obj->GetRect()))
		{
			m_radian = 0;

			obj->Used();

			isHit = true;

			break;
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
		if (m_objects.size() >= kDamageObjectNum) return isHit;

		m_objects.push_back(std::make_shared<BossDamageObject>(m_windowSize, m_fieldSize));
	}
}

void BossStrongArmored::NormalDraw() const
{
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), m_color, true);

	// ダメージオブジェクトの描画
	for (const auto& obj : m_objects)
	{
		obj->Draw();
	}

#ifdef _DEBUG
	// 当たり判定の描画
	m_col.Draw(0xff0000, false);
#endif
}

void BossStrongArmored::CreateEnemy()
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
			Vec2 vec = m_conversionVec.GetNormalized();
			// 現在の角度を得る
			float radian = atan2f(vec.x, vec.y);

			for (int i = 0; i < kCreateNum; i++)
			{
				std::shared_ptr<EnemySplitTwoBound> split;
				split = std::make_shared<EnemySplitTwoBound>(m_windowSize, m_fieldSize);
				split->Init(m_pos, vec);

				m_stage->GenericEnemy(split);

				// 回転させる
				radian += kCreateRadian;

				vec.x = cosf(radian);
				vec.y = sinf(radian);
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
