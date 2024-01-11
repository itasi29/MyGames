#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyMoveWall.h"

namespace
{
	// 初めに生成する敵の数
	constexpr int kStartCreatNum = 4;
	// 初めの生成間隔フレーム
	constexpr int kStartCreateFrame = 10;

	// 敵生成間隔フレーム
	constexpr int kCreateFrame = 60 * 6;

	// 左クリア条件　生存時間
	constexpr int kLeftExsitTime = 10;
	// 上クリア条件　生存時間
	constexpr int kUpExsitTime = 15;

	// 左の部屋の名前
	const std::string kLeftStName = "Stage1-2";
	// 上の部屋の名前
	const std::string kUpStName = "Stage1-3";
}

Stage1_1::Stage1_1(GameManager& mgr) :
	StageBase(mgr),
	m_createFrame(0)
{
	m_stageName = "Stage1-1";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);
	// 1-1に関しては初めからクリアしていることとする
	m_mgr.GetStage()->SaveClear(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_1::~Stage1_1()
{
}

void Stage1_1::Init()
{
	// 経過時間の初期化
	m_frame = 0;
	// 経過を行うかを初期化
	m_isUpdateTime = true;

	// 生成フレームの初期化
	m_createFrame = 0;
	// 生成数の初期化
	m_createNum = 0;

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

	// 敵を一体追加
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
	m_createNum++;
}

void Stage1_1::StartCheck()
{
	m_isLeftClear = m_mgr.GetStage()->IsClearStage(kLeftStName);
	m_isUpClear = m_mgr.GetStage()->IsClearStage(kUpStName);
}

void Stage1_1::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		// 初めに次のステージを作成する
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr);

		SlideLeft(nextStage);

		return;
	}
	if (m_mgr.GetStage()->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr);

		SlideUp(nextStage);

		return;
	}
}

void Stage1_1::CheckStageConditions()
{
	// 左をまだクリアしていない場合
	if (!m_mgr.GetStage()->IsClearStage(kLeftStName))
	{
		// 条件確認
		if (m_mgr.GetStage()->GetBestTime(m_stageName) > kLeftExsitTime * 60)
		{
			m_mgr.GetStage()->SaveClear(kLeftStName);
		}
	}
	// 上をまだクリアしていない場合
	if (!m_mgr.GetStage()->IsClearStage(kUpStName))
	{
		if (m_mgr.GetStage()->GetBestTime(m_stageName) > kUpExsitTime * 60)
		{
			m_mgr.GetStage()->SaveClear(kUpStName);
		}
	}
}

void Stage1_1::DrawStageConditions(int drawY)
{
	if (!m_isLeftClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"左　%d秒間生き残る\n(%d / %d)",
			kLeftExsitTime, m_mgr.GetStage()->GetBestTime(m_stageName) / 60, kLeftExsitTime);

		drawY += 32;
	}
	if (!m_isUpClear)
	{
		DrawFormatString(128, drawY, 0xffffff, L"上　%d秒間生き残る\n(%d / %d)",
			kUpExsitTime, m_mgr.GetStage()->GetBestTime(m_stageName) / 60, kUpExsitTime);
	}
}

void Stage1_1::DrawArrow() const
{
	DrawLeftArrow(m_isLeftClear, kLeftStName);
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_1::DrawKilledEnemyType() const
{
	if (m_mgr.GetStage()->IsKilledEnemy("Normal"))
	{
		DrawCircle(256, 28, 16, 0xffffff, true);
	}
	else
	{
		DrawCircle(256, 28, 16, 0xffffff, false);
	}

	if (m_mgr.GetStage()->IsKilledEnemy("MoveWall"))
	{
		DrawCircle(256 + 48, 28, 16, 0x888888, true);
	}
	else
	{
		DrawCircle(256 + 48, 28, 16, 0x888888, false);
	}
}

void Stage1_1::CreateEnemy()
{
	// 生成時間の更新
	m_createFrame++;

	// 初めは生成間隔が早め
	if (m_createNum < kStartCreatNum && m_createFrame > kStartCreateFrame)
	{
		CreateNormal();

		// 生成すう増加
		m_createNum++;

		return;
	}

	if (m_createFrame > kCreateFrame)
	{
		CreateNormal();
	}
}

void Stage1_1::CreateNormal()
{
	// 生成時間の初期化
	m_createFrame = 0;
	// 配列の最後に敵を追加
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_1::UpdateTime()
{
	m_frame++;
}

