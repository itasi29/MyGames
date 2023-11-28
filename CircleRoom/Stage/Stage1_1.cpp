#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_1.h"
#include "Stage1_2.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"

namespace
{
	// 敵生成間隔フレーム
	constexpr int kCreateFrame = 60 * 5;
}

Stage1_1::Stage1_1(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);
	
	Init();
}

Stage1_1::~Stage1_1()
{
}

void Stage1_1::Init()
{
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

void Stage1_1::CreateEnemy()
{
	// 生成時間の更新
	m_createFrame++;

	if (m_createFrame > kCreateFrame)
	{
		// 生成時間の初期化
		m_createFrame = 0;
		// 配列の最後に敵を追加
		m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));
		// そいつに初期化処理
		float centerX = m_windowSize.w * 0.5f;
		float centerY = m_windowSize.h * 0.5f;
		m_enemy.back()->Init(Vec2{ centerX, centerY });
	}
}

void Stage1_1::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	if (input.IsPress("left"))
	{
		m_mgr->ChangeStage(std::make_shared<Stage1_2>(m_mgr, m_windowSize, m_fieldSize));
	}
}
