#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "Stage1_4.h"
#include "Stage1_3.h"
#include "Stage1_5.h"

#include "Player/Player.h"

#include "Enemy/EnemyMoveWall.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyCreate.h"

namespace
{
	// クリア時間
	constexpr int kRightKilledNum = 3;
	constexpr int kUpKilledNum = 5;

	// 生成間隔
	constexpr int kCreateNormalFrame = 60 * 4;
	constexpr int kCreateCreateFrame = 60 * 7;

	const std::string kRightStName = "Stage1-3";
	const std::string kUpStName = "Stage1-5";
}

Stage1_4::Stage1_4(GameManager& mgr) :
	StageBase(mgr),
	m_createNormalFrame(0)
{
	m_stageName = "Stage1-4";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_4::~Stage1_4()
{
}

void Stage1_4::Init()
{
	// 経過時間の初期化
	m_frame = 0;
	// 経過を行うかを初期化
	m_isUpdateTime = true;

	// 生成フレームの初期化
	m_createNormalFrame = 0;
	m_createCreateFrame = 0;

	// プレイヤーの初期化
	m_player->Init();

	// 敵の配列を初期化
	m_enemy.clear();

	// 壁動く敵の作成
	Vec2 vec;
	// 上側
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_size, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	// 下側
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_size, m_fieldSize));
	vec.y = 1;
	m_enemy.back()->Init(vec);

	m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);

	m_enemy.push_back(std::make_shared<EnemyCreate>(m_size, m_fieldSize, this));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_4::StartCheck()
{
	m_isRightClear = m_mgr.GetStage()->IsClearStage(kRightStName);
	m_isUpClear = m_mgr.GetStage()->IsClearStage(kUpStName);
}

void Stage1_4::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr);

		SlideRight(nextStage);

		return;
	}
	if (m_mgr.GetStage()->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_5> nextStage;
		nextStage = std::make_shared<Stage1_5>(m_mgr);

		SlideUp(nextStage);

		return;
	}
}

void Stage1_4::CheckStageConditions()
{
	// 右をまだクリアしていない場合
	if (!m_mgr.GetStage()->IsClearStage(kRightStName))
	{
		if (m_mgr.GetStage()->GetEnemyTypeCount() >= kRightKilledNum)
		{
			m_mgr.GetStage()->SaveClear(kRightStName);
		}
	}
	if (!m_mgr.GetStage()->IsClearStage(kUpStName))
	{
		if (m_mgr.GetStage()->GetEnemyTypeCount() >= kUpKilledNum)
		{
			m_mgr.GetStage()->SaveClear(kUpStName);
		}
	}
}

void Stage1_4::DrawStageConditions(int drawY)
{
	if (!m_isRightClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"右　%d種類の敵に殺される\n(%d / %d)",
			kRightKilledNum, m_mgr.GetStage()->GetEnemyTypeCount(), kRightKilledNum);

		drawY += 32;
	}
	if (!m_isUpClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"上　%d種類の敵に殺される\n(%d / %d)",
			kUpKilledNum, m_mgr.GetStage()->GetEnemyTypeCount(), kUpKilledNum);
	}
}

void Stage1_4::DrawArrow() const
{
	DrawRightArrow(m_isRightClear, kRightStName);
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_4::DrawKilledEnemyType() const
{
	if (m_mgr.GetStage()->IsKilledEnemy("Normal"))
	{
		DrawCircle(256, 28, 16, 0xffffff, true);
	}
	else
	{
		DrawCircle(256, 28, 16, 0xffffff, true);
	}

	if (m_mgr.GetStage()->IsKilledEnemy("MoveWall"))
	{
		DrawCircle(256 + 48, 28, 16, 0x888888, true);
	}
	else
	{
		DrawCircle(256 + 48, 28, 16, 0x888888, false);
	}

	if (m_mgr.GetStage()->IsKilledEnemy("Create"))
	{
		DrawCircle(256 + 96, 28, 16, 0xffff08, true);
	}
	else
	{
		DrawCircle(256 + 96, 28, 16, 0xffff08, false);
	}

	if (m_mgr.GetStage()->IsKilledEnemy("Child"))
	{
		DrawCircle(256 + 144, 28, 12, 0xf0f008, true);
	}
	else
	{
		DrawCircle(256 + 144, 28, 12, 0xf0f008, false);
	}
}

void Stage1_4::CreateEnemy()
{
	m_createNormalFrame++;
	m_createCreateFrame++;

	if (m_createNormalFrame > kCreateNormalFrame)
	{
		m_createNormalFrame = 0;
		m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
		m_enemy.back()->Init(m_centerPos);
	}

	if (m_createCreateFrame > kCreateCreateFrame)
	{
		m_createCreateFrame = 0;
		m_enemy.push_back(std::make_shared<EnemyCreate>(m_size, m_fieldSize, this));
		m_enemy.back()->Init(m_centerPos);
	}
}

void Stage1_4::UpdateTime()
{
	m_frame++;
}
