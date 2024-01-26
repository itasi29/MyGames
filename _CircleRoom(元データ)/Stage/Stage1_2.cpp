#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "Stage1_1.h"
#include "Stage1_2.h"

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
	constexpr unsigned int kYellowColor = 0xffde00;

	// 条件の描画基準位置
	constexpr int kConditionsPosX = 20;

	// 右側生存時間
	constexpr int kRightExsitTime = 10;

	// 殺された種類の基準描画位置
	constexpr int kKillTypePosX = 156;
	constexpr int kKillTypePosY = 200;

	// 大きい敵生成間隔フレーム
	constexpr int kCreateLageFrame = 60 * 10;
	// 通常的生成間隔フレーム
	constexpr int kCreateNormalFrame = 60 * 5 + 10;

	const std::string kRightStName = "Stage1-1";
}

Stage1_2::Stage1_2(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createLageFrame(0)
{
	m_stageName = "Stage1-2";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);
	CheckStageConditions();

	StartCheck();
}

Stage1_2::~Stage1_2()
{
}

void Stage1_2::Init()
{
	// 経過時間の初期化
	m_frame = 0;
	// 経過を行うかを初期化
	m_isUpdateTime = true;

	// 生成フレームの初期化
	m_createLageFrame = 0;
	m_createNormalFrame = 0;

	// プレイヤーの初期化
	m_player->Init();

	// 敵の配列を初期化
	m_enemy.clear();

	// 壁動く敵の作成
	Vec2 vec;
	// 上側
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_size, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	// 下側
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_size, m_fieldSize));
	vec.y = 1;
	m_enemy.back()->Init(vec);

	// 大きい敵生成
	CreateLage();
	// 通常敵生成
	CreateNormal();
	CreateNormal();
}

void Stage1_2::StartCheck()
{
	m_isRightClear = m_mgr.GetStage()->IsClearStage(kRightStName);
}

void Stage1_2::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kRightStName) && input.IsTriggered("right"))
	{
		// 初めに次のステージを作成する
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		SlideRight(nextStage);

		return;
	}
}

void Stage1_2::CheckStageConditions()
{
	// 右をまだクリアしていない場合
	if (!m_mgr.GetStage()->IsClearStage(kRightStName))
	{
		// 条件確認
		if (m_mgr.GetStage()->GetBestTime(m_stageName) > kRightExsitTime * 60)
		{
			m_mgr.GetStage()->SaveClear(kRightStName);
			AddAchivedStr(L"右");
		}
	}
}

int Stage1_2::DrawStageConditions(int drawY)
{
	int startY = drawY;
	if (!m_isRightClear)
	{
		int fontHandle = m_mgr.GetFont()->GetHandle(28);

		DrawArrowConditions(kRightStName, kConditionsPosX, drawY, kRad90);
		DrawTimeConditions(kConditionsPosX, drawY, fontHandle, kRightExsitTime);

		drawY += 68;
	}

	return drawY - startY - 68;
}

void Stage1_2::DrawArrow() const
{
	DrawRightArrow(m_isRightClear, kRightStName);
}

void Stage1_2::DrawKilledEnemyType() const
{
	if (m_mgr.GetStage()->IsKilledEnemy("Normal"))
	{
		DrawCircle(kKillTypePosX, kKillTypePosY, 16, 0xffffff, true);
	}
	else
	{
		DrawCircle(kKillTypePosX, kKillTypePosY, 16, 0xffffff, false);
	}

	if (m_mgr.GetStage()->IsKilledEnemy("MoveWall"))
	{
		DrawCircle(kKillTypePosX + 36, kKillTypePosY, 16, 0x888888, true);
	}
	else
	{
		DrawCircle(kKillTypePosX + 36, kKillTypePosY, 16, 0x888888, false);
	}

	if (m_mgr.GetStage()->IsKilledEnemy("Large"))
	{
		DrawCircle(kKillTypePosX + 76, kKillTypePosY, 20, 0xffffff, true);
	}
	else
	{
		DrawCircle(kKillTypePosX + 76, kKillTypePosY, 20, 0xffffff, false);
	}
}

void Stage1_2::CreateEnemy()
{
	m_createNormalFrame++;
	m_createLageFrame++;

	if (m_createNormalFrame > kCreateNormalFrame)
	{
		CreateNormal();
	}

	if (m_createLageFrame > kCreateLageFrame)
	{
		CreateLage();
	}
}

void Stage1_2::CreateNormal()
{
	// 生成時間の初期化
	m_createNormalFrame = 0;
	// 配列の最後に敵を追加
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_2::CreateLage()
{
	m_createLageFrame = 0;
	m_enemy.push_back(std::make_shared<EnemyLarge>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
}

void Stage1_2::UpdateTime()
{
	m_frame++;
}
