#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_2.h"
#include "Stage1_1.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"

Stage1_2::Stage1_2(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = L"Stage1-2";

	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);
}

Stage1_2::~Stage1_2()
{
}

void Stage1_2::CheckStageConditions()
{
}

void Stage1_2::DrawStageConditions(bool isPlaying)
{
	DrawFormatString(128, 48, 0xffffff, L"条件");
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

	if (input.IsPress("right"))
	{
		// 初めに次のステージを作成する
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, m_windowSize, m_fieldSize);

		SlideRight(nextStage);
	}
}
