#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_2.h"
#include "Stage1_1.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"

namespace
{
	constexpr int kRightExsitTime = 10;
}

Stage1_2::Stage1_2(StageManager& mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = L"Stage1-2";
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);

	m_clearData.isClears[StageManager::kStageRight] = false;

	m_mgr.GetClearInf("1-2", m_clearData);
}

Stage1_2::~Stage1_2()
{
	SaveInf();
}

void Stage1_2::CheckStageConditions()
{
	// 右をまだクリアしていない場合
	if (!m_clearData.isClears[StageManager::kStageRight])
	{
		// 条件確認
		if (m_frame > kRightExsitTime * 60)
		{
			m_clearData.isClears[StageManager::kStageRight] = true;
		}
	}
}

void Stage1_2::DrawStageConditions(bool isPlaying)
{
	if (isPlaying)
	{
		DrawFormatString(128, 64, 0xffffff, L"右　%d秒間生き残る\n(%d / %d)", kRightExsitTime, m_clearData.bestTime / 60, kRightExsitTime);
	}
	else
	{
		DrawFormatString(128, 48, 0xffffff, L"右　%d秒間生き残る\n(%d / %d)", kRightExsitTime, m_clearData.bestTime / 60, kRightExsitTime);
	}
}

void Stage1_2::Init()
{
	// 経過時間の初期化
	m_frame = 0;

	// 生成フレームの初期化
	m_createFrame = 0;

	// プレイヤーの初期化
	m_player->Init();

	// 敵の配列を初期化
	m_enemy.clear();
	// 敵を一体追加
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));

	// スタート位置の設定
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;
	Vec2 center{ centerX, centerY };

	m_enemy.back()->Init(center);
}

void Stage1_2::CreateEnemy()
{
}

void Stage1_2::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_clearData.isClears[StageManager::kStageRight] && input.IsPress("right"))
	{
		// 初めに次のステージを作成する
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, m_windowSize, m_fieldSize);

		SlideRight(nextStage);
	}
}

void Stage1_2::SaveInf() const
{
	m_mgr.SaveClearInf("1-2", m_clearData);
}
