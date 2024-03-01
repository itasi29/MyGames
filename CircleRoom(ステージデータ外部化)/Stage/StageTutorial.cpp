#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Input.h"

#include "GameManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/SoundSystem.h"
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

	const std::string kStageName = "サークル";
	constexpr int kExsitTime = 2;

	// 生成時間
	constexpr int kCreateFrame = 300;

	// 条件達成時の描画時間("○の条件達成の文字")
	constexpr int kAchivedFrame = 120;

	// サウンドのフェードフレーム
	constexpr int kSoundFade = 30;

	// プレイヤー死亡時の画面の揺れフレーム
	constexpr int kPlayerDeathShakeFrame = 10;

	// プレイヤー強調描画フレーム
	constexpr int kEmphasisFrame = 60 * 5;
	// 透明開始フレーム
	constexpr int kAlphaEmphasisFrame = 60 * 4;
	// 色変更間隔
	constexpr int kEmlhasisInterval = 20;

	// 揺れ角度
	float kShakeAngle = DX_PI_F / 180 * 8;
}

StageTutorial::StageTutorial(GameManager& mgr, Input& input) :
	StageBase(mgr, input),
	m_index(0),
	m_explanation(Explanation::kOperation),
	m_createFrame(0),
	m_isStart(true),
	m_emphasisFrame(0)
{
	m_stageName = "練習";

	// データの生成
	m_mgr.GetStage()->CreateData(m_stageName);

	Init();
}

StageTutorial::~StageTutorial()
{
	// 終了する際にチュートリアル完了という事にする
	m_mgr.GetStage()->SaveClear(m_stageName);
}


void StageTutorial::ChangeStage(Input& input)
{
	if (m_player->IsExsit()) return;

	// 死亡直後は変わらないようにする
	if (m_waitFrame < kWaitChangeFrame) return;


	if (m_mgr.GetStage()->IsClearStage(kStageName) && input.IsTriggered("up"))
	{
		std::shared_ptr<Stage1_1> nextStage = std::make_shared<Stage1_1>(m_mgr, input);

		m_mgr.GetStage()->ChangeStage(nextStage);

		return;
	}
}
