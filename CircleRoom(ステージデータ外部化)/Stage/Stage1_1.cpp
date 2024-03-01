#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileBase.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_4.h"

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
	constexpr int kStartCreatNum = 4;
	// 初めの生成間隔フレーム
	constexpr int kStartCreateFrame = 10;

	// 敵生成間隔フレーム
	constexpr int kCreateFrame = 60 * 6;

	// 左クリア条件　生存時間
	constexpr int kLeftExsitTime = 5;
	// 上クリア条件　生存時間
	constexpr int kUpKilledNum = 4;

	// 左の部屋の名前
	const std::string kLeftStName = "巨壁";
	// 上の部屋の名前
	const std::string kUpStName = "切断";
}

Stage1_1::Stage1_1(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createFrame(0)
{
	m_stageName = "サークル";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

Stage1_1::~Stage1_1()
{
}

void Stage1_1::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	auto& stage = m_mgr.GetStage();
	if (stage->IsClearStage(kLeftStName) && input.IsTriggered("left"))
	{
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
	if (stage->IsClearStage(kUpStName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		stage->ChangeStage(nextStage);

		return;
	}
}
