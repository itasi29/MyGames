#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"
#include "Stage1_9.h"
#include "Stage1_6.h"

#include "Player/Player.h"

namespace
{
	// ラジアンでの90度
	constexpr double kRad90 = DX_PI / 2;

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// 黄色文字列の色
	constexpr unsigned int kYellowColor = 0xffde00;;

	// 初めに生成する敵の数
	constexpr int kCreatDashNum = 2;
	constexpr int kCreatEneCreateNum = 4;
	constexpr int kCreateLargeNum = 3;

	// 生成間隔
	constexpr int kCreateDashInterval = static_cast<int>(60 * 6.5);
	constexpr int kCreateEneCreateInterval = 60 * 12;
	constexpr int kCreateDivisionInterval = 60 * 8;
	constexpr int kCreateLargeInterval = 60 * 4;

	// ディレイフレーム
	constexpr int kStartDeleyFrame = 40;
	constexpr int kDeleyFrame = 60 * 6;

	// 上クリア条件　生存時間
	constexpr int kUpExistTime = 5;

	// 上の部屋の名前
	const std::string kUpStName = "発生";
}

Stage1_9::Stage1_9(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "要警戒";

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);
	Init();
}

Stage1_9::~Stage1_9()
{
}

void Stage1_9::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_6> nextStage;
		nextStage = std::make_shared<Stage1_6>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}
