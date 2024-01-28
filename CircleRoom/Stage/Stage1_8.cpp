#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"

#include "Player/Player.h"
#include "Stage1_8.h"

namespace
{
	// ラジアンでの90度
	constexpr double kRad90 = DX_PI / 2;

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;

	// クリア時間
	constexpr int kDownExsitTime = 15;
	constexpr int kLeftKilledNum = 3;

	// 生成間隔フレーム
	constexpr int kCreateEneCreateFrame = 60 * 14;
	constexpr int kCreateDashFrame = 60 * 13;

	// ディレイフレーム
	constexpr int kDeleyFrame1 = 60 * -3;
	constexpr int kDeleyFrame2 = 60 * -6;

	// 生成数
	constexpr int kCreateNum = 2;

	const std::string kDownStName = "Stage1-1";
	const std::string kLeftStName = "Stage1-4";
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
	m_isUpdateTime = true;

	m_createEneCreateFrame1 = 0;
	m_createEneCreateFrame2 = 0;
	m_createDashFrame = 0;

	m_createNum = 0;

	m_player->Init();
	m_enemy.clear();

	CreateMoveWall();
	CreateDash(m_createDashFrame, true);
}

void Stage1_8::StartCheck()
{
}

void Stage1_8::ChangeStage(Input& input)
{
}

void Stage1_8::CheckStageConditions()
{
}

int Stage1_8::DrawStageConditions(int drawY)
{
	return 0;
}

void Stage1_8::DrawArrow() const
{
}

void Stage1_8::DrawKilledEnemyType() const
{
}

void Stage1_8::CreateEnemy()
{
	m_createDashFrame++;
	m_createEneCreateFrame1++;
	m_createEneCreateFrame2++;

	if (m_createDashFrame > kCreateDashFrame)
	{
		CreateDash(m_createDashFrame);
	}

	if (m_createNum < kCreateNum)
	{
		m_createNum++;
		CreateEneCreate(m_createEneCreateFrame1, true);
		m_createEneCreateFrame1 = kDeleyFrame1;
		m_createEneCreateFrame2 = kDeleyFrame2;
	}
	else
	{
		if (m_createEneCreateFrame1 > kCreateEneCreateFrame)
		{
			CreateEneCreate(m_createEneCreateFrame1);
		}
		if (m_createEneCreateFrame2 > kCreateEneCreateFrame)
		{
			CreateEneCreate(m_createEneCreateFrame2);
		}
	}
}

void Stage1_8::UpdateTime()
{
	m_frame++;
}
