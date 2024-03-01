#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

#include "Scene/OneShotScene.h"
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
	// 強調文字列の色
	constexpr unsigned int kYellowColor = 0xffde00;

	// 条件の描画基準位置
	constexpr int kConditionStrPosX = 20;
	// 生成間隔フレーム
	constexpr int kCreateFrame = static_cast<int>(60 * 5.5);

	// クリア時間
	constexpr int kRightExsitTime = 10;
	constexpr int kDownKilledNum = 4;

	const std::string kRightStName = "分離";
	const std::string kDownStName = "切断";
}

Stage1_8::Stage1_8(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "Reaper";

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

Stage1_8::~Stage1_8()
{
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

void Stage1_8::UniqueEndProcessing()
{
	const auto& armored = std::dynamic_pointer_cast<BossArmored>(m_boss);

	armored->DeleteDamageObjects();
}

void Stage1_8::CreateStrongBoss()
{
	std::shared_ptr<BossStrongArmored> strong;
	strong = std::make_shared<BossStrongArmored>(m_size, m_fieldSize, this);
	strong->Init(m_boss->GetPos());

	m_boss = strong;
}
