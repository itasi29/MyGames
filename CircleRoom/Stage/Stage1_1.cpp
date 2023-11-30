#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageManager.h"
#include "Stage1_1.h"
#include "Stage1_2.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyMoveWall.h"

namespace
{
	// 敵生成間隔フレーム
	constexpr int kCreateFrame = 60 * 5;
}

Stage1_1::Stage1_1(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	StageBase(mgr, windowSize, fieldSize),
	m_createFrame(0)
{
	m_stageName = L"Stage1-1";

	m_player = std::make_shared<Player>(m_windowSize, m_fieldSize);
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

	// 壁動く敵の作成
	Vec2 vec;
	// 上側
	m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	vec.x = 0;
	vec.y = -1;
	m_enemy.back()->Init(vec);
	//// 下側
	//m_enemy.push_back(std::make_shared<EnemyMoveWall>(m_windowSize, m_fieldSize));
	//vec.y = 1;
	//m_enemy.back()->Init(vec);

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

	if (input.IsPress("left"))
	{
		// FIXME:ここで作った奴を後から関数化する

		// 初めに次のステージを作成する
		std::shared_ptr<Stage1_2> nextStage;
		nextStage = std::make_shared<Stage1_2>(m_mgr, m_windowSize, m_fieldSize);

		// FIXME:今からくそコード書くから後で直して /**/があるところまで
		
		// 現在の画面を保存するよう
		int nowScreenHandle;
		nowScreenHandle = MakeScreen(m_windowSize.w, m_windowSize.h, true);
		SetDrawScreen(nowScreenHandle);
		// 現在の画面を描画
		Draw();

		// 送る用の描画先を作成する
		int sendScreenHandle;
		sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h, true);
		// 描画先を作ったスクリーンにする
		SetDrawScreen(sendScreenHandle);
		// 次のステージの選択画面を描画
		nextStage->Draw();
		// ずらして保存した現在の画面を描画
		DrawGraph(m_windowSize.w, 0, nowScreenHandle, true);
		// 描画先を元の場所に戻す
		SetDrawScreen(DX_SCREEN_BACK);

		// 現在の画面を保存したハンドルは宙づりになるのでここで消す
		// 送る方は送ったほうで消すため考えない
		DeleteGraph(nowScreenHandle);

		// 画面を動かす処理を実行する
		m_mgr->StartMove(StageManager::kDirLeft, sendScreenHandle);

		/**/

		// 次のステージに変更する
		m_mgr->ChangeStage(nextStage);
	}
}
