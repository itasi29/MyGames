#include <DxLib.h>
#include <cmath>
#include <memory>

#include "Stage/StageBase.h"
#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"

#include "EnemyDivision.h"
#include "EnemySplit.h"

namespace
{
	// 動くスピード
	constexpr float kSpeed = 4.0f;
	// 半径
	constexpr float kRadius = 43.0f;

	// カラー
	constexpr int kColor = 0xffffff;

	// 分裂する数
	constexpr int kDivisionNum = 4;
	// 分裂するまでのフレーム
	constexpr int kDivisionFrame = 60 * 8;

	// 分裂する前の待機フレーム
	constexpr int kDivisionWaitFrame = 60;
	// 分裂前回転フレーム
	constexpr int kRotationFrame = 40;
}

EnemyDivision::EnemyDivision(const size& windowSize, float fieldSize) :
	EnemyBase(windowSize, fieldSize)
{
	m_name = "Division";
	m_color = kColor;
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Division.png");
}

EnemyDivision::EnemyDivision(const size& windowSize, float fieldSize, StageBase* stage) :
	EnemyBase(windowSize, fieldSize),
	m_stage(stage),
	m_isDivisionWait(false),
	m_divisionWaitFrame(0)
{
	m_name = "Division";
	m_color = kColor;
	m_radius = kRadius;

	auto& mgr = GameManager::GetInstance().GetFile();
	m_charImg = mgr->LoadGraphic(L"Enemy/Division.png");
}

EnemyDivision::~EnemyDivision()
{
}

void EnemyDivision::Init(const Vec2& pos, bool isStart)
{
	// 引数で渡された位置を初期位置に
	m_pos = pos;
	m_radius = kRadius;

	// フレームの初期化
	m_frame = 0;

	// 撃つ方向をランダムで決める
	do
	{
		float moveX = (GetRand(16) - 8) * 0.125f;
		float moveY = (GetRand(16) - 8) * 0.125f;

		if (isStart)
		{
			double angle = atan2(moveX, -moveY);

			if (angle >= (DX_PI / 180 * 135)) continue;
			if (angle <= -(DX_PI / 180 * 135)) continue;
		}


		m_vec = Vec2{ moveX, moveY };
	} while (false);

	// 回転初期化
	m_angle = GetRand(359) * kRad;

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

void EnemyDivision::StartUpdate()
{
	m_frame++;

	if (m_frame > kApeearFrame)
	{
		// 変わるときに当たり判定も入れる
		m_col.SetCenter(m_pos, m_radius);
		auto& sound = GameManager::GetInstance().GetSound();
		sound->PlaySe(m_createSe->GetHandle());

		EnemyBase::ChangeNormalFunc();

		m_frame = 0;
	}
}

void EnemyDivision::NormalUpdate()
{
	// 分裂待機状態なら以下の処理を
	if (m_isDivisionWait)
	{
		m_divisionWaitFrame++;
		if (m_divisionWaitFrame < kRotationFrame)
		{
			m_angle -= (kRad * 20);
		}

		// 待機時間を超えたら分裂する
		if (m_divisionWaitFrame > kDivisionWaitFrame)
		{
			m_isExsit = false;

			// ベクトルを保存する
			Vec2 vec = { cosf(static_cast<float>(m_angle)), sinf(static_cast<float>(m_angle)) };
			for (int i = 0; i < kDivisionNum; i++)
			{
				std::shared_ptr<EnemySplit> split;
				split = std::make_shared<EnemySplit>(m_size, m_fieldSize);
				split->Init(m_pos, vec);

				m_stage->GenericEnemy(split);

				// 回転させる
				vec = vec.Right();
			}
		}

		return;
	}

	m_pos += m_vec;
	m_angle -= kRad;
	Reflection();

	m_col.SetCenter(m_pos, m_radius);

	m_frame++;

	// 一定時間超えたら分裂前状態に入る
	if (m_frame > kDivisionFrame)
	{
		m_isDivisionWait = true;
	}
}
