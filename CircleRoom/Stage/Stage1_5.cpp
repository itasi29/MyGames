#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_5.h"
#include "Stage1_4.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Boss/BossArmored.h"
#include "Boss/BossStrongArmored.h"

namespace
{
	constexpr int kDownKilledNum = 5;
}

Stage1_5::Stage1_5(StageManager& mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = "Stage1-5";
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);

	// データの生成
	m_mgr.CreateData(m_stageName);

	m_isDownClear = m_mgr.IsClearStage(m_stageName, StageManager::kStageDown);
}

Stage1_5::~Stage1_5()
{
}

void Stage1_5::Init()
{
	// 経過時間の初期化
	m_frame = 0;
	// 経過を行うかを初期化
	m_isUpdateTime = false;

	// 生成フレームの初期化
	m_createFrame = 0;

	// プレイヤーの初期化
	m_player->Init();

	// 敵の配列を初期化
	m_enemy.clear();

	// 壁動く敵の作成
	Vec2 vec;
	// 上側
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	// 下側
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.y = 1;
	m_enemy.back()->Init(vec);

	// スタート位置の設定
	m_boss = std::make_shared<BossArmored>(m_windowSize, m_fieldSize, this);
	m_boss->Init(m_centerPos);
}

void Stage1_5::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.IsClearStage(m_stageName, StageManager::kStageDown) && input.IsPress("down"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, m_windowSize, m_fieldSize);

		SlideDown(nextStage);

		return;
	}
}

void Stage1_5::CheckStageConditions()
{
	// 下をまだクリアしていない場合
	if (!m_mgr.IsClearStage(m_stageName, StageManager::kStageDown))
	{
		if (m_mgr.GetEnemyTypeCount() >= kDownKilledNum)
		{
			m_mgr.SaveClear(m_stageName, StageManager::kStageRight);
		}
	}
}

void Stage1_5::DrawStageConditions(int drawY)
{
	if (!m_isDownClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"下　%dの種類で死ぬ\n(%d / %d)",
			kDownKilledNum, m_mgr.GetEnemyTypeCount(), kDownKilledNum);

		drawY += 32;
	}

	// FIXME: ここに追加で書いているけれどあとで別のところに処理を変更する
	if (m_mgr.IsClearBoss("BossArmored"))
	{
		DrawString(128, drawY, L"clear", 0xffffff);
	}
}

void Stage1_5::DrawArrow() const
{
	DrawDownArrow();
}

void Stage1_5::CreateEnemy()
{
}

void Stage1_5::UpdateTime()
{
	if (m_isUpdateTime)
	{
		// 一秒追加
		m_frame += 60.0f;
		m_isUpdateTime = false;
	}
}

void Stage1_5::CreateStrongBoss()
{
	std::shared_ptr<BossStrongArmored> strong;
	strong = std::make_shared<BossStrongArmored>(m_windowSize, m_fieldSize, this);
	strong->Init(m_boss->GetPos());

	m_boss = strong;
}
