#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "StageManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileManager.h"
#include "Stage1_5.h"
#include "Stage1_4.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"
#include "Boss/BossArmored.h"
#include "Boss/BossStrongArmored.h"

namespace
{
	// 通常文字列の色
	constexpr unsigned int kStrColor = 0xf0ece5;

	// 殺された種類の基準描画位置
	constexpr int kKillTypePosX = 144;
	constexpr int kKillTypePosY = 152;

	constexpr int kDownKilledNum = 5;

	const std::string kDownStName = "Stage1-4";
}

Stage1_5::Stage1_5(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_createFrame(0)
{
	m_stageName = "Stage1-5";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// 1-5はボスのためBGMを変更する
	m_playBgm = m_mgr.GetFile()->LoadSound(L"Bgm/boss.mp3");

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);

	StartCheck();
}

Stage1_5::~Stage1_5()
{
}

void Stage1_5::Init()
{
	// 経過時間の初期化
	m_frame = 0;
	// 経過を行うかを初期化
	m_isUpdateTime = false;

	// 生成フレームの初期化
	m_createFrame = 0;

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

	// スタート位置の設定
	m_boss = std::make_shared<BossArmored>(m_size, m_fieldSize, this);
	m_boss->Init(m_centerPos);
}

void Stage1_5::StartCheck()
{
	m_isDownClear = m_mgr.GetStage()->IsClearStage(kDownStName);
}

void Stage1_5::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_mgr.GetStage()->IsClearStage(kDownStName) && input.IsTriggered("down"))
	{
		std::shared_ptr<Stage1_4> nextStage;
		nextStage = std::make_shared<Stage1_4>(m_mgr, input);

		SlideDown(nextStage);

		return;
	}
}

void Stage1_5::CheckStageConditions()
{
	// 下をまだクリアしていない場合
	if (!m_mgr.GetStage()->IsClearStage(kDownStName))
	{
		if (m_mgr.GetStage()->GetEnemyTypeCount() >= kDownKilledNum)
		{
			m_mgr.GetStage()->SaveClear(kDownStName);
		}
	}
}

int Stage1_5::DrawStageConditions(int drawY)
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(24);

	if (!m_isDownClear)
	{
		DrawFormatStringToHandle(128, drawY, kStrColor, fontHandle, L"下　%d種類の敵に\n　　殺される\n(%d / %d)",
			kDownKilledNum, m_mgr.GetStage()->GetEnemyTypeCount(), kDownKilledNum);

		drawY += 72;
	}

	// FIXME: ここに追加で書いているけれどあとで別のところに処理を変更する
	if (m_mgr.GetStage()->IsClearBoss("BossArmored"))
	{
		DrawStringToHandle(128, drawY, L"clear", kStrColor, fontHandle);

		drawY += 48;
	}
	else
	{
		DrawStringToHandle(128, drawY, L"ボスを倒せ！", kStrColor, fontHandle);

		drawY += 72;
	}

	return drawY - startY - 48;
}

void Stage1_5::DrawArrow() const
{
	DrawDownArrow(m_isDownClear, kDownStName);
}

void Stage1_5::DrawKilledEnemyType() const
{
	if (m_mgr.GetStage()->IsKilledEnemy("MoveWall"))
	{
		DrawCircle(kKillTypePosX, kKillTypePosY, 16, 0x888888, true);
	}
	else
	{
		DrawCircle(kKillTypePosX, kKillTypePosY, 16, 0x888888, true);
	}

	if (m_mgr.GetStage()->IsKilledEnemy("BossArmored"))
	{
		DrawCircle(kKillTypePosX + 48, kKillTypePosY, 16, 0x08ff08, true);
	}
	else
	{
		DrawCircle(kKillTypePosX + 48, kKillTypePosY, 16, 0x08ff08, false);
	}

	if (m_mgr.GetStage()->IsKilledEnemy("BossStrongArmored"))
	{
		DrawCircle(kKillTypePosX + 96, kKillTypePosY, 16, 0xaaffaa, true);
	}
	else
	{
		DrawCircle(kKillTypePosX + 96, kKillTypePosY, 16, 0xaaffaa, false);
	}

	if (m_mgr.GetStage()->IsKilledEnemy("SplitTwoBound"))
	{
		DrawCircle(kKillTypePosX + 144, kKillTypePosY, 14, 0xffffff, true);
	}
	else
	{
		DrawCircle(kKillTypePosX + 144, kKillTypePosY, 14, 0xffffff, false);
	}
}

void Stage1_5::CreateEnemy()
{
}

void Stage1_5::UpdateTime()
{
	if (m_isUpdateTime)
	{
		// 一秒追加
		m_frame += 60;
		m_isUpdateTime = false;
	}
}

void Stage1_5::CreateStrongBoss()
{
	std::shared_ptr<BossStrongArmored> strong;
	strong = std::make_shared<BossStrongArmored>(m_size, m_fieldSize, this);
	strong->Init(m_boss->GetPos());

	m_boss = strong;
}
