#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "Stage1_3.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyMoveWall.h"

namespace
{
	// 敵生成間隔フレーム
	constexpr int kCreateFrame = 60 * 5;

	// 左クリア条件　生存時間
	constexpr int kLeftExsitTime = 10;
	// 上クリア条件　生存時間
	constexpr int kUpExsitTime = 15;
}

Stage1_1::Stage1_1(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = L"Stage1-1";

	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);

	m_mgr->GetClearInf("1-1", m_isClear);
}

Stage1_1::~Stage1_1()
{
	m_mgr->SaveClearInf("1-1", m_isClear);
}

void Stage1_1::CheckStageConditions()
{
	// 左をまだクリアしていない場合
	if (!m_isClear[0])
	{
		// 条件確認
		if (m_frame > kLeftExsitTime * 60)
		{
			m_isClear[0] = true;
		}
	}
	// 上をまだクリアしていない場合
	if (!m_isClear[1])
	{
		if (m_frame > kUpExsitTime * 60)
		{
			m_isClear[1] = true;
		}
	}
}

void Stage1_1::DrawStageConditions(bool isPlaying)
{
	if (isPlaying)
	{
		DrawFormatString(128, 64, 0xffffff, L"左　%d秒間生き残る\n(%d / %d)", kLeftExsitTime, m_bestTime / 60, kLeftExsitTime);
		DrawFormatString(128, 96, 0xffffff, L"上　%d秒間生き残る\n(%d / %d)", kUpExsitTime, m_bestTime / 60, kUpExsitTime);
	}
	else
	{
		DrawFormatString(128, 48, 0xffffff, L"左　%d秒間生き残る\n(%d / %d)", kLeftExsitTime, m_bestTime / 60, kLeftExsitTime);
		DrawFormatString(128, 80, 0xffffff, L"上　%d秒間生き残る\n(%d / %d)", kUpExsitTime, m_bestTime / 60, kUpExsitTime);
	}
}

void Stage1_1::Init()
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

	// 敵を一体追加
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_windowSize, m_fieldSize));

	// スタート位置の設定
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;
	vec = { centerX, centerY };

	m_enemy.back()->Init(vec);
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
		Vec2 center(m_windowSize.w * 0.5f, m_windowSize.h * 0.5f);
		m_enemy.back()->Init(center);
	}
}

void Stage1_1::ChangeStage(Input& input)
{
	// プレイヤーが生存している間は変わらないようにする
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;

	if (m_isClear[0] && input.IsPress("left"))
	{
		// 初めに次のステージを作成する
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, m_windowSize, m_fieldSize);

		SlideLeft(nextStage);
		
		return;
	}
	if (m_isClear[1] && input.IsPress("up"))
	{
		std::shared_ptr<Stage1_3> nextStage;
		nextStage = std::make_shared<Stage1_3>(m_mgr, m_windowSize, m_fieldSize);

		SlideUp(nextStage);

		return;
	}
}
