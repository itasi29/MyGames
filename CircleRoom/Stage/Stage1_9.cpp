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
	constexpr int kStartCreatNum = 4;
	// 初めの生成間隔フレーム
	constexpr int kStartCreateFrame = 10;

	// 敵生成間隔フレーム
	constexpr int kCreateFrame = 60 * 6;

	// 上クリア条件　生存時間
	constexpr int kUpExistTime = 5;

	// 上の部屋の名前
	const std::string kUpStName = "Stage1-6";
}

Stage1_9::Stage1_9(GameManager& mgr, Input& input) :
	StageBase(mgr, input)
{
	m_stageName = "Stage1-9";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);
	// 1-1に関しては初めからクリアしていることとする
	//m_mgr.GetStage()->SaveClear(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_9::~Stage1_9()
{
}

void Stage1_9::Init()
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
	CreateMoveWall();
}

void Stage1_9::StartCheck()
{
	auto& stage = m_mgr.GetStage();
	m_isUpClear = stage->IsClearStage(kUpStName);
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

void Stage1_9::CheckStageConditions()
{
	CheckConditionsTime(kUpStName, kUpExistTime, L"上");
}

int Stage1_9::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);
	if (!m_isUpClear)
	{
		DrawArrowConditions(kUpStName, drawY, -kRad90);
		DrawTimeConditions(drawY, fontHandle, kUpExistTime);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_9::DrawArrow() const
{
	DrawUpArrow(m_isUpClear, kUpStName);
}

void Stage1_9::DrawKilledEnemyType() const
{
}

void Stage1_9::CreateEnemy()
{
}

void Stage1_9::UpdateTime()
{
	m_frame++;
}
