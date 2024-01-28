#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_3.h"
#include "Stage1_4.h"
#include "Stage1_2.h"
#include "Stage1_5.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Enemy/EnemyDash.h"

namespace
{
	// ラジアンでの90度
	constexpr double kRad90 = DX_PI / 2;

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// 生成間隔フレーム
	constexpr int kCreateFrame = 60 * 5.5;

	// 生成数
	constexpr int kCreateNum = 2;

	// クリア時間
	constexpr int kRightKilledNum = 4;
	constexpr int kDownExsitTime = 10;
	constexpr int kUpExsitTime = 10;


	const std::string kRightStName = "Stage1-4";
	const std::string kDownStName = "Stage1-2";
	const std::string kUpStName = "Stage1-5";
}

Stage1_3::Stage1_3(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createFrame(0)
{
	m_stageName = "Stage1-3";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_3::~Stage1_3()
{
}

void Stage1_3::Init()
{
	// 経過時間の初期化
	m_frame = 0;
	// 経過を行うかを初期化
	m_isUpdateTime = true;

	// 生成フレームの初期化
	m_createFrame = 0;

	// 生成数初期化
	m_createNum = 0;

	// プレイヤーの初期化
	m_player->Init();

	// 敵の配列を初期化
	m_enemy.clear();

	// 壁動く敵の作成
	CreateMoveWall();
}

void Stage1_3::StartCheck()
{
	auto& stage = m_mgr.GetStage();
	m_isRightClear = stage->IsClearStage(kRightStName);
	m_isDownClear = stage->IsClearStage(kDownStName);
	m_isUpClear = stage->IsClearStage(kUpStName);
}

void Stage1_3::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_5> nextStage;
		nextStage = std::make_shared<Stage1_5>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}

void Stage1_3::CheckStageConditions()
{
	CheckConditionsKilled(kRightStName, kRightKilledNum, L"右");
	CheckConditionsTime(kDownStName, kDownExsitTime, L"下");
	CheckConditionsTime(kUpStName, kUpExsitTime, L"上");
}

int Stage1_3::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);

	if (!m_isRightClear)
	{
		DrawArrowConditions(kRightStName, drawY, kRad90);
		DrawKilledConditions(drawY, fontHandle, kRightKilledNum);

		drawY += 68;
	}
	if (!m_isDownClear)
	{
		DrawArrowConditions(kDownStName, drawY, DX_PI);
		DrawTimeConditions(drawY, fontHandle, kDownExsitTime);

		drawY += 68;
	}
	if (!m_isUpClear)
	{
		DrawArrowConditions(kUpStName, drawY, 0.0);
		DrawTimeConditions(drawY, fontHandle, kUpExsitTime);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_3::DrawArrow() const
{
	DrawRightArrow(m_isRightClear, kRightStName);
	DrawDownArrow(m_isDownClear, kDownStName);
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_3::DrawKilledEnemyType() const
{
	DrawKilledEnemy("Dash", 0, 0x0808ff);

	DrawKilledEnemy("MoveWall", 36, 0x888888);
}

void Stage1_3::CreateEnemy()
{
	m_createFrame++;

	if (m_createNum < kCreateNum)
	{
		m_createNum++;
		CreateDash(m_createFrame, true);
		return;
	}

	if (m_createFrame > kCreateFrame)
	{
		CreateDash(m_createFrame);
	}
}

void Stage1_3::UpdateTime()
{
	m_frame++;
}
