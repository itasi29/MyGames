#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "StageManager.h"
#include "Scene/SceneManager.h"
#include "Scene/OneShotScene.h"

#include "Player/Player.h"
#include "Enemy/EnemyNormal.h"

#include "StageTutorial.h"
#include "Stage1_1.h"

namespace
{
	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;

	// 条件の描画基準位置
	constexpr int kConditionsPosX = 20;

	const std::string kStageName = "Stage1-1";
	constexpr int kExsitTime = 2;

	// 生成時間
	constexpr int kCreateFrame = 300;


	// プレイヤー死亡時の画面の揺れフレーム
	constexpr int kShakeFrameDeath = 10;
}

StageTutorial::StageTutorial(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_explanation(kOperation),
	m_createFrame(0)
{
	m_handle[kOperation] = m_mgr.GetFile()->LoadGraphic(L"UI/operationExplanation.png");
	m_handle[kClear] = m_mgr.GetFile()->LoadGraphic(L"UI/clearExplanation.png");
	m_handle[kClearAnother] = m_mgr.GetFile()->LoadGraphic(L"UI/clearExplanation.png");

	m_stageName = "Tutorial";
	m_player = std::make_shared<Player>(m_size, m_fieldSize);

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);
}

StageTutorial::~StageTutorial()
{
	// 終了する際にチュートリアル完了という事にする
	m_mgr.GetStage()->SaveClear(m_stageName);
}

void StageTutorial::Init()
{
	m_frame = 0;
	m_createFrame = 0;
	m_isUpdateTime = true;

	m_player->Init();

	m_enemy.clear();
	m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
	m_enemy.back()->Init(m_centerPos);
}

void StageTutorial::ChangeStage(Input& input)
{
	if (m_player->IsExsit()) return;

	if (m_mgr.GetStage()->IsClearStage(kStageName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_1> nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}

void StageTutorial::UpdateSelect(Input& input)
{
	m_waitFrame++;

	m_player->Update(input, kNone);

	for (auto& enemy : m_enemy)
	{
		enemy->Update();
	}

	switch (m_explanation)
	{
		// 説明1つ目
	case kOperation:
		m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_handle[m_explanation]->GetHandle()));
		m_explanation = kClear;
		break;
		// 説明2つ目
	case kClear:
		m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_handle[m_explanation]->GetHandle()));
		m_explanation = kPlay;
		break;
		// 実プレイ
	case kPlay:
		if (input.IsTriggered("OK"))
		{
			ChangePlayingFunc();
			Init();
		}
		break;
		// クリア後説明
	case kClearAnother:
		m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_handle[m_explanation]->GetHandle()));
		m_explanation = kEnd;
		break;
		// 特になし
	default:
		assert(false);
	case kEnd:
		break;;
	}
}

void StageTutorial::UpdatePlaying(Input& input)
{
	m_player->Update(input, m_mgr.GetStage()->GetAbility());

	// プレイヤーの情報を抜き取る
	bool playerIsDash = m_player->IsDash();
	bool playerIsExsit = m_player->IsExsit();
	const Collision& playerCol = m_player->GetRect();

	CreateEnemy();
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();

		// プレイヤーとの当たり判定処理
		// ダッシュしていたら処理はしない
		if (!playerIsDash && playerCol.IsCollsion(enemy->GetRect()))
		{
			// プレイヤーの死亡処理
			m_player->Death();
			m_mgr.GetScene()->ShakeScreen(kShakeFrameDeath);

			// 殺したことがある敵情報の更新
			m_mgr.GetStage()->UpdateEnemyType(enemy->GetName());

			break;
		}
	}

	// 死亡した敵は消す
	m_enemy.remove_if(
		[](const auto& enemy)
		{
			return !enemy->IsExsit();
		});

	UpdateTime();

	if (!m_player->IsExsit())
	{
		ChangeSelectFunc();

		// フレームの初期化
		m_waitFrame = 0;

		// ベストタイムの更新
		m_mgr.GetStage()->UpdateBestTime(m_stageName, m_frame);

		// クリアしているかの確認
		CheckStageConditions();
	}
}

void StageTutorial::CheckStageConditions()
{
	if (m_mgr.GetStage()->GetBestTime(m_stageName) > kExsitTime * 60)
	{
		m_mgr.GetStage()->SaveClear(kStageName);
		m_explanation = kClearAnother;
	}
}

int StageTutorial::DrawStageConditions(int drawY)
{
	DrawTimeConditions(kConditionsPosX, drawY, m_mgr.GetFont()->GetHandle(28), kExsitTime);

	return 0;
}

void StageTutorial::DrawArrow() const
{
	DrawUpArrow(false, kStageName);
}

void StageTutorial::CreateEnemy()
{
	m_createFrame++;

	if (m_createFrame > kCreateFrame)
	{
		m_createFrame = 0;

		m_enemy.push_back(std::make_shared<EnemyNormal>(m_size, m_fieldSize));
		m_enemy.back()->Init(m_centerPos);
	}
}

void StageTutorial::UpdateTime()
{
	m_frame++;
}
