#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"

#include "Player/Player.h"
#include "Stage1_7.h"
#include "Stage1_8.h"
#include "Stage1_6.h"

namespace
{
	// ラジアンでの90度
	constexpr double kRad90 = DX_PI / 2;

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// 生成間隔フレーム
	constexpr int kCreateFrame = 60 * 10;

	// ディレイフレーム
	constexpr int kDeleyFrame = 60 * -4;

	// 生成数
	constexpr int kCreateNum = 3;


	// クリア時間
	constexpr int kDownExsitTime = 10;
	constexpr int kLeftExsitTime = 60;

	const std::string kLeftStName = "Reaper";
	const std::string kDownStName = "発生";

	const std::vector<std::string> kNames =
	{
		"サークル",
		"巨壁",
		"近接遭遇",
		"切断",
		"ランナー",
		"発生",
		"分離"
	};
}

Stage1_7::Stage1_7(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "分離";

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);
	Init();
}

Stage1_7::~Stage1_7()
{
}

void Stage1_7::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_8> nextStage;
		nextStage = std::make_shared<Stage1_8>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_6> nextStage;
		nextStage = std::make_shared<Stage1_6>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}
