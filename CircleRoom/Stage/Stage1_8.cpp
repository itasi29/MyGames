#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"

#include "Player/Player.h"
#include "Boss/BossStrongArmored.h"
#include "Boss/BossArmored.h"
#include "Stage1_8.h"
#include "Stage1_7.h"
#include "Stage1_4.h"

namespace
{
	// ラジアンでの90度
	constexpr double kRad90 = DX_PI / 2;

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;

	// 条件の描画基準位置
	constexpr int kConditionsPosX = 20;
	// 生成間隔フレーム
	constexpr int kCreateFrame = static_cast<int>(60 * 5.5);

	// 生成数
	constexpr int kCreateNum = 2;


	// クリア時間
	constexpr int kRightExsitTime = 10;
	constexpr int kDownExsitTime = 15;

	const std::string kRightStName = "Stage1-4";
	const std::string kDownStName = "Stage1-7";
}

Stage1_8::Stage1_8(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "Stage1-8";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_8::~Stage1_8()
{
}

void Stage1_8::Init()
{
	m_frame = 0;
	m_isUpdateTime = false;

	m_player->Init();
	m_enemy.clear();

	CreateMoveWall();

	m_boss = std::make_shared<BossArmored>(m_size, m_fieldSize, this);
	m_boss->Init(m_centerPos);
}

void Stage1_8::StartCheck()
{
	m_isRightClear = m_mgr.GetStage()->IsClearStage(kRightStName);
	m_isDownClear = m_mgr.GetStage()->IsClearStage(kDownStName);
}

void Stage1_8::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		std::shared_ptr<Stage1_7> nextStage;
		nextStage = std::make_shared<Stage1_7>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}

void Stage1_8::CheckStageConditions()
{
	CheckConditionsTime(kRightStName, kRightExsitTime, L"右");
	CheckConditionsKilled(kDownStName, kDownExsitTime, L"下");
}

int Stage1_8::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);

	if (!m_isRightClear)
	{
		DrawArrowConditions(kRightStName, drawY, -kRad90);
		DrawTimeConditions(drawY, fontHandle, kRightExsitTime);

		drawY += 68;
	}
	if (!m_isDownClear)
	{
		DrawArrowConditions(kDownStName, drawY, DX_PI);
		DrawKilledConditions(drawY, fontHandle, kDownExsitTime);

		drawY += 68;
	}

	if (m_mgr.GetStage()->IsClearBoss("BossArmored"))
	{
		DrawStringToHandle(kConditionsPosX, drawY + 14, L"clear", kWhiteColor, fontHandle);

		drawY += 70;
	}
	else
	{
		DrawStringToHandle(kConditionsPosX, drawY + 14, L"ボスを倒せ！", kWhiteColor, fontHandle);

		drawY += 70;
	}

	return drawY - startY - 68;
}

void Stage1_8::DrawArrow() const
{
	DrawRightArrow(m_isRightClear, kRightStName);
	DrawDownArrow(m_isDownClear, kDownStName);
}

void Stage1_8::DrawKilledEnemyType() const
{
	DrawKilledEnemy("MoveWall", 0, 0xb6bbc4);
	DrawKilledEnemy("BossArmored", 40, 0x294b29, 20);
	DrawKilledEnemy("BossStrongArmored", 84, 0xdbe7c9, 20);
	DrawKilledEnemy("SplitTwoBound", 118, 0xd5f0c1, 12);
}

void Stage1_8::CreateEnemy()
{
}

void Stage1_8::CreateStrongBoss()
{
	std::shared_ptr<BossStrongArmored> strong;
	strong = std::make_shared<BossStrongArmored>(m_size, m_fieldSize, this);
	strong->Init(m_boss->GetPos());

	m_boss = strong;
}

void Stage1_8::UpdateTime()
{
	if (m_isUpdateTime)
	{
		// 一秒追加
		m_frame += 60;
		m_isUpdateTime = false;
	}
}
