#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_5.h"
#include "Stage1_3.h"

#include "Player/Player.h"

namespace
{
	// ラジアンでの90度
	constexpr double kRad90 = DX_PI / 2;

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;

	constexpr int kCreateFrame = 60 * 3;
	constexpr int kCreateNum = 5;


	constexpr int kDownExsitTime = 10;

	const std::string kDownStName = "近接遭遇";
}

Stage1_5::Stage1_5(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createFrame(0)
{
	m_stageName = "ランナー";

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

Stage1_5::~Stage1_5()
{
}

void Stage1_5::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}
