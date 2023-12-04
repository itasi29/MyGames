#include <DxLib.h>
#include "Input.h"

#include "Stage1_3.h"
#include "Stage1_1.h"

#include "Player/Player.h"
#include "Enemy/EnemyMoveWall.h"

Stage1_3::Stage1_3(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = L"Stage1-3";

	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);
}

Stage1_3::~Stage1_3()
{
}

void Stage1_3::CheckStageConditions()
{
}

void Stage1_3::DrawStageConditions(bool isPlaying)
{
	DrawFormatString(128, 48, 0xffffff, L"条件");
}

void Stage1_3::Init()
{
	// 経過時間の初期化
	m_frame = 0;

	// 生成フレームの初期化
	m_createFrame = 0;

	// プレイヤーの初期化
	m_player->Init();

	// 敵の配列を初期化
	m_enemy.clear();

	// 壁動く敵の作成
	Vec2 vec;
	// 上側
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	// 下側
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.y = 1;
	m_enemy.back()->Init(vec);
}

void Stage1_3::CreateEnemy()
{
	// とりあえずなにも生成しない
}

void Stage1_3::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	if (input.IsPress("down"))
	{
		std::shared_ptr<Stage1_1> nextStage;
		nextStage = std::make_shared<Stage1_1>(m_mgr, m_windowSize, m_fieldSize);

		SlideDown(nextStage);
	}
}
