#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyLarge.h"

#include"Enemy/EnemyDivision.h"

namespace
{
	// ラジアンでの90度
	constexpr double kRad90 = DX_PI / 2;

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// 黄色文字列の色
	constexpr unsigned int kYellowColor = 0xffde00;;
	// 大きい敵生成間隔フレーム
	constexpr int kCreateLageFrame = 60 * 10;
	// 通常的生成間隔フレーム
	constexpr int kCreateNormalFrame = 60 * 10;
	// 生成ディレイフレーム1
	constexpr int kDeleyFrame1 = 60 * -3;
	// 生成ディレイフレーム2
	constexpr int kDeleyFrame2 = 60 * -5;

	// 通常初回生成数(Normal)
	constexpr int kCreateNum = 2;


	constexpr int kRightExsitTime = 5;
	constexpr int kUpExsitTime = 10;

	const std::string kRightStName = "サークル";
	const std::string kUpStName = "近接遭遇";
}

Stage1_2::Stage1_2(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createLageFrame(0)
{
	m_stageName = "巨壁";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

Stage1_2::~Stage1_2()
{
}

void Stage1_2::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		// 初めに次のステージを作成する
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		// 初めに次のステージを作成する
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}

void Stage1_2::UpdateTime()
{
	m_timeFrame++;
}
