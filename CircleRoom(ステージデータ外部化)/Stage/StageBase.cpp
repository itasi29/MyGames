#include <DxLib.h>
#include <cassert>
#include <fstream>
#include <sstream>
#include "Application.h"
#include "Input.h"
#include "StringUtility.h"

#include "StageBase.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "StageManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/BottansFile.h"
#include "FileSystem/KeyFile.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyMoveWall.h"
#include "Enemy/EnemyNormal.h"
#include "Enemy/EnemyLarge.h"
#include "Enemy/EnemyDash.h"
#include "Enemy/EnemyCreate.h"
#include "Enemy/EnemyDivision.h"
#include "Boss/BossBase.h"

#include "Boss/BossArmored.h"
#include "Boss/BossStrongArmored.h"
#include "Scene/OneShotScene.h"

namespace
{
	// ステージデータインデックス番号
	constexpr int kIndexStageName = 0;
	constexpr int kIndexUpdateTimeType = 1;
	constexpr int kIndexAddTimeFrame = 2;
	constexpr int kIndexEnemyTypeNum = 3;
	constexpr int kIndexEnemyName = 4;
	constexpr int kIndexEnemyInfoNum = 5;
	constexpr int kIndexStartCreateNum = 6;
	constexpr int kIndexStartCreateFrame = 7;
	constexpr int kIndexStartDelayFrame = 8;
	constexpr int kIndexCreateFrame = 9;
	constexpr int kIndexIsCreateBoss = 10;
	constexpr int kIndexNextStageNum = 11;
	constexpr int kIndexNextStageName = 12;
	constexpr int kIndexDir = 13;
	constexpr int kIndexInfoType = 14;
	constexpr int kIndexInfo = 15;
	constexpr int kIndexInfoGroupNum = 16;
	constexpr int kIndexInfoGroup = 17;
	// 説明インデックス番号
	constexpr int kIndexBossExplanation = 5;
	// チュートリアル説明総数
	constexpr int kExplanationNum = 5;

	// フィールドサイズの倍率　(半分の大きさ)
	constexpr float kSizeScale = 0.4f;

	// プレイヤー死亡時の画面の揺れフレーム
	constexpr int kPlayerDeathShakeFrame = 10;

	// サウンドのフェードフレーム
	constexpr int kSoundFade = 30;

	// 色定数
	constexpr unsigned int kFrameColor = 0xd80032;
	constexpr unsigned int kBackFrameColor = 0x161a30;
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	constexpr unsigned int kYellowColor = 0xffde00;
	constexpr unsigned int kRedColor = 0xd2001a;

	// 矢印画像定数
	constexpr int kArrowFlashInterval = 20;
	constexpr double kRad90 = DX_PI / 2;

	// ウェーブ文字定数
	constexpr float kWaveSpeed = DX_PI_F / 180 * 5;
	constexpr float kWaveInterval = DX_PI_F / 15.0f;
	int kTitleWaveNum = 4;
	const wchar_t* const kTitleWave[] = {L"ス", L"タ", L"ー", L"ト"};
	int kDashWaveNum = 4;
	const wchar_t* const kDashWave[] = {L"ダ", L"ッ", L"シ", L"ュ"};

	// 拡大文字定数
	constexpr float kExtRateSize = 2.0f;
	constexpr int kExtRateFrame = 45;
	constexpr int kWaitExtRateFrame = 30;
	constexpr int kExtRateAlpha = 224;
	constexpr int kExtShiftWidth = 256;
	constexpr int kExtShiftRight = 320;
	constexpr int kExtShiftUp = 360;

	// ステージ名座標定数
	constexpr int kNamePosX = 64;
	constexpr int kNamePosY = 32;
	constexpr int kNameFramePosX = 0;
	constexpr int kNameFramePosY = 24;
	constexpr int kNameFrameWidth = 288;
	constexpr int kNameFrameHeight = 80;

	// 時間座標定数
	constexpr int kTimePosX = 20;
	constexpr int kTimePosY = 160;
	constexpr int kTimePlayingDiff = -16;
	constexpr int kTimeAddY = 24;

	// 敵種類定数
	constexpr int kKilledStrPosX = 136;
	constexpr int kKilledStrPosY = 160;
	constexpr int kKillTypePosX = 156;
	constexpr int kKillTypePosY = 200;
	constexpr int kKillTypeAlpha = 192;
	constexpr double kKillTypeDefExtRate = 0.372;
	constexpr double kKillTypeLargeExtRate = 0.6;
	const std::vector<std::string> kLargeTypeName = {
		"Child",
		"Split",
		"SplitTwoBound"
	};
	constexpr double kKillTypeSmallExtRate = 0.25;
	const std::vector<std::string> kSmallTypeName = {
		"Large",
		"BossArmored",
		"BossStrongArmored"
	};

	// 情報定数
	constexpr int kInfoFramePosX = 155;
	constexpr int kInfoFramePosY = 168;
	constexpr int kInfoBoxPosX = 0;
	constexpr int kInfoBoxPosY = 198;
	constexpr int kInfoBoxWidth = 310;
	constexpr int kInfoBoxHeight = 30;
	constexpr int kInfoBoxPlayingDiff = 6;

	// 条件定数
	constexpr int kConditionStrPosX = 20;
	constexpr int kConditionPlayingDiff = 24;
	constexpr int kConditionPosY = 256;
	constexpr int kConditionBoxPosX = 0;
	constexpr int kConditionBoxPosY = 244;
	constexpr int kConditionBoxWidth = 310;

	// 条件側矢印定数
	constexpr int kArrowSize = 28;
	// ステージ側矢印定数
	constexpr int kArrowShiftPos = 150;

	// １秒のフレーム数
	constexpr int kFrameToSec = 60;

	// ベストタイム座標定数
	constexpr int kBestTimeFrameSubX = 128;
	constexpr int kBestTimeFramePosY = 128;
	constexpr int kBestTimeFrameBoxPosY = 158;
	constexpr int kBestTimeFrameBoxSizeW = 155;
	constexpr int kBestTimeFrameBoxSizeH = 66;
	constexpr int kBestTimeStrSubX = 256;
	constexpr int kBestTimeStrPosY = 112;
	constexpr int kBestTimeStrAddY = 48;
	constexpr int kBestTimeFlashInterval = 30;
	constexpr int kBestTimeBoxPosX = 344;
	constexpr int kBestTimeBoxPosY = 248;
	constexpr int kBestTimeBoxHeight = 64;
	constexpr int kBestTimeUpStrPosX = 336;
	constexpr int kBestTimeUpStrPosY = 256;

	// 達成文字描画定数
	constexpr int kAchivedDrawFrame = 120;
	constexpr int kAchivedStrShiftPosY = 344;
	constexpr int kAchivedStrPosX = 12;
	constexpr int kAchivedFrameWidth = 352;
	constexpr int kAchivedFrameHeight = 68;
	constexpr int kAchivedStrAdd = 64;
	constexpr int kAchivedFrameDiff = 4;

	// ウェーブ文字描画定数
	constexpr int kWaveFramePosX = 980;
	constexpr int kWaveFramePosY = 595;
	constexpr int kWaveStrPosX = 1064;
	constexpr int kWaveStrPosY = 600;
	constexpr int kWaveStrAdd = 24;
	constexpr int kWaveImgPosX = 1016;
	constexpr int kWaveImgPosY = 600;

	/* チュートリアル関連定数 */
	// 自機指し矢印
	constexpr int kEmphasisFrame = 60 * 5;
	constexpr int kStartAlphaEmphasisFrame = 60 * 4;
	constexpr int kEmphasisInterval = 20;
	constexpr float kEmphasisAngle = DX_PI_F / 180 * 8;
}

StageBase::StageBase(GameManager& mgr, Input& input) :
	m_mgr(mgr),
	m_soundSys(m_mgr.GetSound()),
	m_input(input),
	m_size(Application::GetInstance().GetWindowSize()),
	m_fieldSize(m_size.h* kSizeScale),
	m_centerPos({m_size.w * 0.5f, m_size.h * 0.5f}),
	m_stageName({}),
	m_achived({}),
	m_soundFrame(kSoundFade),
	m_timeFrame(0),
	m_waitFrame(kWaitChangeFrame),
	m_extRateFrame(0),
	m_isUpdateBestTime(false),
	m_isExtRate(true),
	m_isWaveDraw(true),
	m_waveAngle(DX_PI_F),
	m_explanationIndex(0),
	m_emphasisFrame(0)
{
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;

	// ステージ情報読み込み
	LoadStageInfo();

	// 画面の作成
	m_strScreen = MakeScreen(m_size.w, m_size.h, true);
	m_extScreen = MakeScreen(m_size.w, m_size.h, true);

	// 読み込み
	auto& file = m_mgr.GetFile();
	// 画像
	m_fieldImg = file->LoadGraphic(L"field.png");
	m_arrowImg = file->LoadGraphic(L"UI/arrow.png");
	m_arrowFlashImg = file->LoadGraphic(L"UI/arrowFlash.png");
	m_arrowNoImg = file->LoadGraphic(L"UI/arrowNo.png");
	m_arrowLockImg = file->LoadGraphic(L"UI/lock.png");
	m_arrowConditionsImg = file->LoadGraphic(L"UI/arrowConditions.png");
	m_frameImg = file->LoadGraphic(L"UI/startFrame.png");
	m_backFrameImg = file->LoadGraphic(L"UI/backFrame.png");
	m_enemysImg["Normal"] = file->LoadGraphic(L"Enemy/Normal.png");
	m_enemysImg["MoveWall"] = file->LoadGraphic(L"Enemy/Wall.png");
	m_enemysImg["Large"] = file->LoadGraphic(L"Enemy/Large.png");
	m_enemysImg["Division"] = file->LoadGraphic(L"Enemy/Division.png");
	m_enemysImg["Dash"] = file->LoadGraphic(L"Enemy/Dash.png");
	m_enemysImg["Create"] = file->LoadGraphic(L"Enemy/Create.png");
	m_enemysImg["Child"] = file->LoadGraphic(L"Enemy/Child.png");
	m_enemysImg["Split"] = file->LoadGraphic(L"Enemy/Split.png");
	m_enemysImg["SplitTwoBound"] = file->LoadGraphic(L"Enemy/SplitTwoBound.png");
	m_enemysImg["BossArmored"] = file->LoadGraphic(L"Enemy/BossArmored.png");
	m_enemysImg["BossStrongArmored"] = file->LoadGraphic(L"Enemy/BossStrongArmored.png");
	m_checkImg = file->LoadGraphic(L"UI/check.png");
	m_explanation.push_back(file->LoadGraphic(L"UI/operationExplanation.png"));
	m_explanation.push_back(file->LoadGraphic(L"UI/explanation0.png"));
	m_explanation.push_back(file->LoadGraphic(L"UI/explanation1.png"));
	m_explanation.push_back(file->LoadGraphic(L"UI/explanation2.png"));
	m_explanation.push_back(file->LoadGraphic(L"UI/explanation3.png"));
	m_explanation.push_back(file->LoadGraphic(L"UI/bossExplanation.png"));
	m_emphasisArrow.push_back(file->LoadGraphic(L"UI/playerEmphasis0.png"));
	m_emphasisArrow.push_back(file->LoadGraphic(L"UI/playerEmphasis1.png"));
	// BGM
	m_selectBgm = file->LoadSound(L"Bgm/provisionalBgm.mp3");
	m_playBgm = file->LoadSound(L"Bgm/fieldFight.mp3");
	// SE
	m_clearSe = file->LoadSound(L"Se/clear.mp3");
	// キー・ボタン
	m_bt = std::make_shared<BottansFile>(file);
	m_key = std::make_shared<KeyFile>(file);

	// プレイヤー生成
	m_player = std::make_shared<Player>(m_size, m_fieldSize);
}

StageBase::~StageBase()
{
	DeleteGraph(m_strScreen);
	DeleteGraph(m_extScreen);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="input">入力情報</param>
void StageBase::Update(Input& input)
{
	// ウェーブ文字更新
	m_isWaveDraw = true;
	m_waveAngle -= kWaveSpeed;
	(this->*m_updateFunc)(input);
}

/// <summary>
/// 描画処理
/// </summary>
void StageBase::Draw() const
{
	(this->*m_drawFunc)();
}

/// <summary>
/// 敵の追加
/// </summary>
void StageBase::GenericEnemy(const std::shared_ptr<EnemyBase>& enemy)
{
	m_backEnemy.push_back(enemy);
}

/// <summary>
/// 選択中の更新処理
/// </summary>
/// <param name="input">入力情報</param>
void StageBase::UpdateSelect(Input& input)
{
	if (m_soundFrame > kSoundFade)
	{
		m_soundSys->PlayBgm(m_selectBgm->GetHandle());
	}
	else
	{
		m_soundFrame++;
		m_soundSys->PlayFadeBgm(m_selectBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));
	}

	m_waitFrame++;

	// プレイヤー更新
	m_player->Update(input, Ability::kNone);
	// 敵更新
	for (const auto& enemy : m_backEnemy)
	{
		enemy->Update();

	}
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();
	}
	for (const auto& enemy : m_frontEnemy)
	{
		enemy->Update();
	}
	// 死亡した敵の処理
	m_backEnemy.remove_if(
		[](const auto& enemy)
		{
			return !enemy->IsExsit();
		});
	m_enemy.remove_if(
		[](const auto& enemy)
		{
			return !enemy->IsExsit();
		});
	m_frontEnemy.remove_if(
		[](const auto& enemy)
		{
			return !enemy->IsExsit();
		});
	// ボス更新
	if (m_boss)
	{
		m_boss->Update();
	}

	// 条件達成文字更新
	for (auto& achived : m_achived)
	{
		achived.frame++;
	}
	m_achived.remove_if(
		[](const auto& achived)
		{
			return achived.frame > kAchivedDrawFrame;
		}
	);

	// ここまではステージ外部化しても同じ処理


#if false
	if (input.IsTriggered("OK"))
	{
		// 移動中であっても即時移動
		m_mgr.GetStage()->ImmediatelyChange();

		// BGM変更
		m_soundFrame = 0;
		m_soundSys->Stop(m_selectBgm->GetHandle());
		m_soundSys->PlayFadeBgm(m_playBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));

		Init();

		m_updateFunc = &StageBase::UpdatePlaying;
		m_drawFunc = &StageBase::DrawPlaying;
	}
#else
	if (UpdateTutorial()) return;

	if (input.IsTriggered("OK"))
	{
		// 移動中であっても即時移動
		m_mgr.GetStage()->ImmediatelyChange();

		// BGM変更
		m_soundFrame = 0;
		m_soundSys->Stop(m_selectBgm->GetHandle());
		m_soundSys->PlayFadeBgm(m_playBgm->GetHandle(), 0);

		PlayStart();

		m_updateFunc = &StageBase::UpdatePlaying;
		m_drawFunc = &StageBase::DrawPlaying;
	}
#endif

	// アビリティ変更
	m_mgr.GetStage()->ChangeAbility(Ability::kDash);
}

/// <summary>
/// プレイ中の更新処理
/// </summary>
/// <param name="input">入力情報</param>
void StageBase::UpdatePlaying(Input& input)
{
	if (m_soundFrame > kSoundFade)
	{
		m_soundSys->PlayBgm(m_playBgm->GetHandle());
	}
	else
	{
		m_soundFrame++;
		m_soundSys->PlayFadeBgm(m_playBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));
	}

	m_emphasisFrame++;
	m_extRateFrame++;
	if (m_isUpdateBestTime)
	{
		// ベストタイムの常時更新
		m_waitFrame++;
		m_mgr.GetStage()->UpdateBestTime(m_stageName, m_timeFrame);
	}
	else if (m_mgr.GetStage()->UpdateBestTime(m_stageName, m_timeFrame))
	{
		m_isUpdateBestTime = true;
	}
	// MEMO:途中で消したい場合はこれをオンに
#if false
	for (auto& achived : m_achived)
	{
		achived.frame++;
	}
	m_achived.remove_if(
		[](const auto& achived)
		{
			return achived.frame > kAchivedFrame;
		}
	);
#endif

	// プレイヤー更新
	m_player->Update(input, m_mgr.GetStage()->GetAbility());
	// プレイヤーの情報を抜き取る
	bool playerIsDash = m_player->IsDash();
	bool playerIsExsit = m_player->IsExsit();
	const Collision& playerCol = m_player->GetRect();
#ifdef _DEBUG
	// デバッグ用無敵モード
	if (CheckHitKey(KEY_INPUT_M))
	{
		playerIsDash = true;
	}
#endif

	// 敵生成
	CreateEnemy();
	// 敵更新
	UpdateEnemy(m_frontEnemy, playerIsDash, playerCol);
	UpdateEnemy(m_enemy, playerIsDash, playerCol);
	UpdateEnemy(m_backEnemy, playerIsDash, playerCol);

	if (m_boss)
	{
		// ボス更新
		m_boss->Update();
		m_boss->OnAttack(playerIsDash, m_player->GetObjRect());

		if (!m_boss->IsExsit())
		{
			DeathBoss();
		}
		else if (!playerIsDash && playerCol.IsCollsion(m_boss->GetRect()))
		{
			m_player->Death();

			m_mgr.UpdateDeathCcount();
			m_mgr.GetStage()->UpdateEnemyType(m_boss->GetName());
			m_mgr.GetScene()->ShakeScreen(kPlayerDeathShakeFrame);
		}
	}

#ifdef _DEBUG
	// デバッグ用として高速で時間が進むように
	if (CheckHitKey(KEY_INPUT_U))
	{
		for (int i = 0; i < 60; i++)
		{
			UpdateTime();
		}
	}
#endif
	UpdateTime();
	CheckStageConditions(m_timeFrame);

	if (!m_player->IsExsit())
	{
		// BGM変更
		m_soundFrame = 0;
		m_soundSys->Stop(m_playBgm->GetHandle());
		m_soundSys->PlayFadeBgm(m_selectBgm->GetHandle(), 0);

		m_waitFrame = 0;
		m_waveAngle = 0.0;
		m_isExtRate = false;

		// ベストタイムの更新
		if (m_mgr.GetStage()->UpdateBestTime(m_stageName, m_timeFrame))
		{
			m_isUpdateBestTime = true;
		}

		m_updateFunc = &StageBase::UpdateSelect;
		m_drawFunc = &StageBase::DrawSelect;

		return;
	}

	DrawTutrial();
}

/// <summary>
/// ボス倒した後の更新処理
/// </summary>
/// <param name="input">入力情報</param>
void StageBase::UpdateAfterBossDeath(Input& input)
{
	if (m_soundFrame > kSoundFade)
	{
		m_soundSys->PlayBgm(m_playBgm->GetHandle());
	}
	else
	{
		m_soundFrame++;
		m_soundSys->PlayFadeBgm(m_playBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));
	}

	m_boss->Update();

	if (m_boss->IsEndPerformance())
	{
		m_mgr.GetStage()->OnClear();
	}
}

/// <summary>
/// 選択中の描画処理
/// </summary>
void StageBase::DrawSelect() const
{
	DrawWall();

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	for (const auto& enemy : m_backEnemy)
	{
		enemy->Draw();
	}
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	for (const auto& enemy : m_frontEnemy)
	{
		enemy->Draw();
	}	
	if (m_boss)
	{
		m_boss->Draw();
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	m_player->Draw();

	auto name = StringUtility::StringToWString(m_stageName);
	int fontHandle = m_mgr.GetFont()->GetHandle(32);
	int drawScreenHandle = m_mgr.GetScene()->GetScreenHandle();

	// ステージ名の描画
	DrawBox(kNameFramePosX, kNameFramePosY, kNameFramePosX + kNameFrameWidth, kNameFramePosY + kNameFrameHeight, kFrameColor, true);
	DrawFormatStringToHandle(kNamePosX, kNamePosY, kYellowColor, m_mgr.GetFont()->GetHandle(64), L"%s", name.c_str());

	// ステージ情報の描画
	SetDrawScreen(m_strScreen);
	ClearDrawScreen();
	// 時間
	DrawTime(kTimePosX, kTimePosY, fontHandle);
	// 種類
	DrawStringToHandle(kKilledStrPosX, kKilledStrPosY, L"> Circle", kWhiteColor, m_mgr.GetFont()->GetHandle(24));
	DrawEnemyKilledInfo(kKillTypePosX, kKillTypePosY);
	SetDrawScreen(drawScreenHandle);
	// フレーム
	DrawRotaGraph(kInfoFramePosX, kInfoFramePosY, 1.0, 0.0, m_backFrameImg->GetHandle(), true, false, true);
	DrawBox(kInfoBoxPosX, kInfoBoxPosY, kInfoBoxPosX + kInfoBoxWidth, kInfoBoxPosY + kInfoBoxHeight, kBackFrameColor, true);
	DrawGraph(0, 0, m_strScreen, true);

	// ステージ条件の描画
	SetDrawScreen(m_strScreen);
	ClearDrawScreen();
	auto y = DrawStageConditions(kConditionPosY);
	SetDrawScreen(drawScreenHandle);
	// Y座標の変動があればフレームを描画
	if (y >= 0)
	{
		DrawBox(kConditionBoxPosX, kConditionBoxPosY, kConditionBoxPosX + kConditionBoxWidth, kConditionBoxPosY + y, kBackFrameColor, true);
		DrawGraph(kConditionBoxPosX, kConditionBoxPosY + y, m_backFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strScreen, true);

	DrawBestTime();
	DrawArrow();
	DrawWave("OK", kTitleWave, kTitleWaveNum);
	DrawConditionsAchived(y);
}

/// <summary>
/// プレイ中の描画処理
/// </summary>
void StageBase::DrawPlaying() const
{
	DrawWall();

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	for (const auto& enemy : m_backEnemy)
	{
		enemy->Draw();
	}
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	for (const auto& enemy : m_frontEnemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	m_player->Draw();

	int drawScreenHandle = m_mgr.GetScene()->GetScreenHandle();

	// ステージ情報の描画
	SetDrawScreen(m_strScreen);
	ClearDrawScreen();
	// 時間
	DrawTime(kTimePosX, kTimePosY + kTimePlayingDiff, m_mgr.GetFont()->GetHandle(64));
	// フレーム
	SetDrawScreen(drawScreenHandle);
	DrawRotaGraph(kInfoFramePosX, kInfoFramePosY, 1.0, 0.0, m_backFrameImg->GetHandle(), true, false, true);
	DrawBox(kInfoBoxPosX, kInfoBoxPosY, kInfoBoxPosX + kInfoBoxWidth, kInfoBoxPosY + kInfoBoxHeight + kInfoBoxPlayingDiff, kBackFrameColor, true);
	DrawGraph(0, 0, m_strScreen, true);

	// 条件の描画
	SetDrawScreen(m_strScreen);
	ClearDrawScreen();
	auto y = DrawStageConditions(kConditionPosY + kConditionPlayingDiff);
	// Y座標の変動があればフレームを描画
	SetDrawScreen(m_extScreen);
	ClearDrawScreen();
	if (y >= 0)
	{
		DrawBox(kConditionBoxPosX, kConditionBoxPosY + kConditionPlayingDiff, kConditionBoxPosX + kConditionBoxWidth, kConditionBoxPosY + kConditionPlayingDiff + y, kBackFrameColor, true);
		DrawGraph(kConditionBoxPosX, kConditionBoxPosY + kConditionPlayingDiff + y, m_backFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strScreen, true);
	// 本体に描画
	SetDrawScreen(drawScreenHandle);
	if (m_isExtRate && m_extRateFrame < kExtRateFrame)
	{
		DrawExpansion();
	}
	else
	{
		DrawGraph(0, 0, m_extScreen, true);
	}

	if (m_mgr.GetStage()->GetAbility() == Ability::kDash)
	{
		DrawWave("dash", kDashWave, kDashWaveNum);
	}

	DrawBestTime();
	DrawUnique();
	DrawConditionsAchived(y + kConditionPlayingDiff);
}

/// <summary>
/// ボス倒した後の描画処理
/// </summary>
void StageBase::DrawAfterBossDeath() const
{
	DrawWall();

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	m_boss->Draw();
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

/// <summary>
/// クリア時間を超えているかの確認
/// </summary>
/// <param name="stageName">確認するステージ名</param>
/// <param name="timeFrame">タイムフレーム</param>
/// <param name="exsitTime">クリア時間(秒)</param>
/// <param name="dir">方向名</param>
void StageBase::CheckConditionsTime(const std::string& stageName, int timeFrame, int exsitTime, const std::wstring& dir)
{
	auto& stage = m_mgr.GetStage();

	if (stage->IsClearStage(stageName)) return;

	if (timeFrame > exsitTime * kFrameToSec)
	{
		m_soundSys->PlaySe(m_clearSe->GetHandle());
		stage->SaveClear(stageName);
		AddAchivedStr(dir);
	}
}

/// <summary>
/// あるステージ全ての生存時間がクリア時間を超えているか
/// </summary>
/// <param name="stageName">確認するステージ名</param>
/// <param name="names">確認するステージの名前群</param>
/// <param name="exsitTime">クリア時間(秒)</param>
/// <param name="dir">方向名</param>
void StageBase::CheckConditionsSumTime(const std::string& stageName, const std::vector<std::string>& names, int timeFrame, int exsitTime, const std::wstring& dir)
{
	auto& stage = m_mgr.GetStage();
	int sumTime = 0;

	if (stage->IsClearStage(stageName)) return;

	// 確認するステージのすべてのタイムを加算する
	for (const auto& name : names)
	{
		if (name == m_stageName)
		{
			sumTime += timeFrame;
		}
		else
		{
			sumTime += stage->GetBestTime(name);
		}
	}

	if (sumTime >= exsitTime * kFrameToSec)
	{
		m_soundSys->PlaySe(m_clearSe->GetHandle());
		stage->SaveClear(stageName);
		AddAchivedStr(dir);
	}
}

/// <summary>
/// 殺された数が超えているかの確認
/// </summary>
/// <param name="stageName">確認するステージ名</param>
/// <param name="killedNum">殺される種類数</param>
/// <param name="dir">方向名</param>
void StageBase::CheckConditionsKilled(const std::string& stageName, int killedNum, const std::wstring& dir)
{
	auto& stage = m_mgr.GetStage();

	if (stage->IsClearStage(stageName)) return;

	if (stage->GetEnemyTypeCount() >= killedNum)
	{
		m_soundSys->PlaySe(m_clearSe->GetHandle());
		stage->SaveClear(stageName);
		AddAchivedStr(dir);
	}
}

/// <summary>
/// クリア条件のタイムを描画
/// </summary>
/// <param name="y">描画Y座標</param>
/// <param name="handle">フォントハンドル</param>
/// <param name="existTime">クリア時間(秒)</param>
void StageBase::DrawTimeConditions(int y, int handle, int existTime) const
{
	DrawStringToHandle(kConditionStrPosX, y, L"　　   秒間生き残る\n　　(           )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionStrPosX, y, kYellowColor, handle, L"　　%2d\n　　  %2d / %2d",
		existTime, m_mgr.GetStage()->GetBestTime(m_stageName) / kFrameToSec, existTime);
}

/// <summary>
/// クリア条件の合計時間のやつ描画
/// </summary>
/// <param name="names">確認するステージ群情報</param>
/// <param name="y">描画Y座標</param>
/// <param name="handle">フォントハンドル</param>
/// <param name="existTime">合計クリア時間(秒)</param>
void StageBase::DrawSumTimeConditions(const std::vector<std::string>& names, int y, int handle, int existTime) const
{
	auto& stage = m_mgr.GetStage();
	int sumTime = 0;

	// 確認するステージのすべてのタイムを加算する
	for (const auto& name : names)
	{
		sumTime += stage->GetBestTime(name);
	}

	// 秒に戻す
	sumTime /= kFrameToSec;

	DrawStringToHandle(kConditionStrPosX, y, L"　　全ステージ合計\n　　    秒間生き残る\n　　(           )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionStrPosX, y, kYellowColor, handle, L"\n　　%02d\n　　  %2d / %2d",
		existTime, sumTime, existTime);
}

/// <summary>
/// クリア条件の敵に殺される種類数を描画
/// </summary>
/// <param name="y">描画Y座標</param>
/// <param name="handle">フォントハンドル</param>
/// <param name="killedNum">倒される種類数</param>
void StageBase::DrawKilledConditions(int y, int handle, int killedNum) const
{
	DrawStringToHandle(kConditionStrPosX, y, L"　　   種類の敵に殺される\n　　(          )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionStrPosX, y, kYellowColor, handle, L"　　%2d\n　　  %2d / %2d",
		killedNum, m_mgr.GetStage()->GetEnemyTypeCount(), killedNum);
}

/// <summary>
/// クリア条件にある矢印の描画
/// </summary>
/// <param name="nextStName">次のステージ名</param>
/// <param name="y">描画Y座標</param>
/// <param name="angle">矢印角度(上基準の時計回り)</param>
/// <param name="isReverseX">反転X</param>
/// <param name="isReverxeY">反転Y/param>
void StageBase::DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX, bool isReverxeY) const
{
	if (m_mgr.GetStage()->IsClearStage(nextStName) && (m_waitFrame / kArrowFlashInterval) % 2 != 0)
	{
		DrawBox(kConditionStrPosX, y, kConditionStrPosX + kArrowSize, y + kArrowSize, 0xffde00, true);
	}
	DrawRotaGraph(kConditionStrPosX + static_cast<int>(kArrowSize * 0.5), y + static_cast<int>(kArrowSize * 0.5), 1.0, angle, m_arrowConditionsImg->GetHandle(), true, isReverseX, isReverxeY);
}

/// <summary>
/// 左矢印の描画
/// </summary>
void StageBase::DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f - kArrowShiftPos);
	int y = static_cast<int>(m_size.h * 0.5f);
	DrawRotaGraph(x, y, 1.0, -kRad90, handle, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

/// <summary>
/// 右矢印の描画
/// </summary>
void StageBase::DrawRightArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f + kArrowShiftPos);
	int y = static_cast<int>(m_size.h * 0.5f);
	// MEMO:何故かReverXをtrueにするとがびらないからしておいてる
	DrawRotaGraph(x, y, 1.0, kRad90, handle, true, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

/// <summary>
/// 上矢印の描画
/// </summary>
void StageBase::DrawUpArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f);
	int y = static_cast<int>(m_size.h * 0.5f - kArrowShiftPos);
	DrawRotaGraph(x, y, 1.0, 0.0, handle, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

/// <summary>
/// 下矢印の描画
/// </summary>
void StageBase::DrawDownArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f);
	int y = static_cast<int>(m_size.h * 0.5f + kArrowShiftPos);
	DrawRotaGraph(x, y, 1.0, 0.0, handle, true, false, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

/// <summary>
/// 敵タイプの描画
/// </summary>
/// <param name="enemyName">名前</param>
/// <param name="addX">X座標の追加値</param>
/// <param name="radius">半径 : def = 16</param>
void StageBase::DrawKilledEnemy(const std::string& enemyName, int x, int y, int addX, int radius) const
{
	double enemyExtRate = kKillTypeDefExtRate;
	// 小さくする敵の場合は小さく
	for (const auto& name : kSmallTypeName)
	{
		if (name == enemyName)
		{
			enemyExtRate = kKillTypeSmallExtRate;
			break;
		}
	}
	// 大きくする敵の場合は大きく
	for (const auto& name : kLargeTypeName)
	{
		if (name == enemyName)
		{
			enemyExtRate = kKillTypeLargeExtRate;
			break;
		}
	}

	auto& file = m_enemysImg.at(enemyName);
	if (m_mgr.GetStage()->IsKilledEnemy(enemyName))
	{
		DrawRotaGraph(x + addX, y, enemyExtRate, 0.0, file->GetHandle(), true);
		DrawRotaGraph(x + addX, y, 0.5, 0.0, m_checkImg->GetHandle(), true);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kKillTypeAlpha);
		DrawRotaGraph(x + addX, y, enemyExtRate, 0.0, file->GetHandle(), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

/// <summary>
/// クリアした文字の描画群に追加
/// </summary>
/// <param name="dir">方向</param>
void StageBase::AddAchivedStr(const std::wstring& dir)
{
	m_achived.push_back({ dir + L"の条件達成！", 0 });
}

void StageBase::CreateMoveWall()
{
	// 上側
	auto enemy = std::make_shared<EnemyMoveWall>(m_size, m_fieldSize);
	enemy->Init({ 0, -1 });
	m_enemy.push_back(enemy);
	// 下側
	enemy = std::make_shared<EnemyMoveWall>(m_size, m_fieldSize);
	enemy->Init({ 0, 1 });
	m_enemy.push_back(enemy);
}

void StageBase::CreateNormal(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyNormal>(m_size, m_fieldSize);
	enemy->Init(m_centerPos, isStart);
	m_enemy.push_back(enemy);
}

void StageBase::CreateLarge(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyLarge>(m_size, m_fieldSize);
	enemy->Init(m_centerPos, isStart);
	m_enemy.push_back(enemy);
}

void StageBase::CreateDash(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyDash>(m_size, m_fieldSize, m_player);
	enemy->Init(m_centerPos, isStart);
	m_frontEnemy.push_back(enemy);
}

void StageBase::CreateEneCreate(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyCreate>(m_size, m_fieldSize, this);
	enemy->Init(m_centerPos, isStart);
	m_enemy.push_back(enemy);
}

void StageBase::CreateDivision(int& frame, bool isStart)
{
	frame = 0;
	auto enemy = std::make_shared<EnemyDivision>(m_size, m_fieldSize, this);
	enemy->Init(m_centerPos, isStart);
	m_frontEnemy.push_back(enemy);
}

void StageBase::ChangeSelectFunc()
{
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;
}

void StageBase::ChangePlayingFunc()
{
	m_updateFunc = &StageBase::UpdatePlaying;
	m_drawFunc = &StageBase::DrawPlaying;
}

/// <summary>
/// 敵の更新
/// </summary>
/// <param name="enemys">敵の配列</param>
/// <param name="isDash">プレイヤーがダッシュしているか</param>
/// <param name="col">プレイヤーの当たり判定</param>
void StageBase::UpdateEnemy(std::list<std::shared_ptr<EnemyBase>>& enemys, bool isDash, const Collision& col)
{
	for (const auto& enemy : enemys)
	{
		enemy->Update();

		if (!isDash && col.IsCollsion(enemy->GetRect()))
		{
			m_player->Death();

			m_mgr.UpdateDeathCcount();
			m_mgr.GetStage()->UpdateEnemyType(enemy->GetName());

			m_mgr.GetScene()->ShakeScreen(kPlayerDeathShakeFrame);

			break;
		}
	}

	enemys.remove_if(
		[](const auto& enemy)
		{
			return !enemy->IsExsit();
		}
	);
}

/// <summary>
/// ボスの死亡処理
/// </summary>
void StageBase::DeathBoss()
{
	// すでにクリアされていた場合は強化ボスを出す
	if (m_mgr.GetStage()->IsClearBoss(m_boss->GetName()))
	{
		CreateStrongBoss();
		return;
	}

	// ベストタイムの更新
	if (m_mgr.GetStage()->UpdateBestTime(m_stageName, m_timeFrame))
	{
		m_isUpdateBestTime = true;
	}

	// クリアしているかの確認
	CheckStageConditions(m_mgr.GetStage()->GetBestTime(m_stageName));

	// 倒した情報の追加
	m_mgr.GetStage()->UpdateClearBoss(m_boss->GetName());

	// 敵全て消す
	m_enemy.clear();
	m_frontEnemy.clear();
	m_backEnemy.clear();

	// 死亡後はダメージオブジェクトをすべて消す
	const auto& armored = std::dynamic_pointer_cast<BossArmored>(m_boss);
	armored->DeleteDamageObjects();

	m_updateFunc = &StageBase::UpdateAfterBossDeath;
	m_drawFunc = &StageBase::DrawAfterBossDeath;
}

/// <summary>
/// 現在のタイムを描画
/// </summary>
/// <param name="x">描画X座標</param>
/// <param name="y">描画Y座標</param>
/// <param name="handle">フォントハンドル</param>
void StageBase::DrawTime(int x, int y, int handle) const
{
	DrawStringToHandle(x, y, L"> タイム", kWhiteColor, m_mgr.GetFont()->GetHandle(24));
	y += kTimeAddY;
	int minSec = (m_timeFrame * 1000 / kFrameToSec) % 1000;
	int sec = (m_timeFrame / kFrameToSec) % kFrameToSec;
	int min = m_timeFrame / (kFrameToSec * kFrameToSec);
	DrawFormatStringToHandle(x, y, kYellowColor, handle, L"%01d:%02d.%03d", min, sec, minSec);
}

/// <summary>
/// ベストタイム関連の描画
/// </summary>
void StageBase::DrawBestTime() const
{
	// フレーム描画
	DrawRotaGraph(m_size.w - kBestTimeFrameSubX, kBestTimeFramePosY, 1.0, 0.0, m_backFrameImg->GetHandle(), true, true, true);
	DrawBox(m_size.w - 128 - 155, 158, m_size.w, 224, kBackFrameColor, true);
	DrawBox(m_size.w - kBestTimeFrameSubX - kBestTimeFrameBoxSizeW, kBestTimeFrameBoxPosY, 
		m_size.w, kBestTimeFramePosY + kBestTimeFrameBoxSizeH, kBackFrameColor, true);
	// ベストタイムの描画
	int bestTime = m_mgr.GetStage()->GetBestTime(m_stageName);
	int minSec = (bestTime * 1000 / kFrameToSec) % 1000;
	int sec = (bestTime / kFrameToSec) % kFrameToSec;
	int min = bestTime / (kFrameToSec * kFrameToSec);
	DrawStringToHandle(m_size.w - kBestTimeStrSubX, kBestTimeStrPosY, L"> ベストタイム", kWhiteColor, m_mgr.GetFont()->GetHandle(32));
	DrawFormatStringToHandle(m_size.w - kBestTimeStrSubX, kBestTimeStrPosY + kBestTimeStrAddY, kYellowColor, m_mgr.GetFont()->GetHandle(64), L"%02d:%02d.%03d", min, sec, minSec);

	if (!m_isUpdateBestTime) return;

	// 点滅描画
	if (((m_waitFrame / kBestTimeFlashInterval) % 2) == 1)
	{
		DrawFormatStringToHandle(m_size.w - kBestTimeStrSubX, kBestTimeStrPosY + kBestTimeStrAddY, kRedColor, m_mgr.GetFont()->GetHandle(64), L"%02d:%02d.%03d", min, sec, minSec);
	}
	
	if (m_waitFrame > kAchivedDrawFrame) return;

	// 更新した場合教える用
	if (m_waitFrame > static_cast<int>(kAchivedDrawFrame * 0.5f))
	{
		float rate = 1.0f - (m_waitFrame - (kAchivedDrawFrame * 0.5f)) / (kAchivedDrawFrame * 0.5f);
		int alpha = static_cast<int>(255 * rate);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(m_size.w - kBestTimeBoxPosX, kBestTimeBoxPosY, m_size.w, kBestTimeBoxPosY + kBestTimeBoxHeight, kBackFrameColor, true);
		DrawStringToHandle(m_size.w - kBestTimeUpStrPosX, kBestTimeUpStrPosY, L"ベストタイム更新！", kYellowColor, m_mgr.GetFont()->GetHandle(48));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		return;
	}

	DrawBox(m_size.w - kBestTimeBoxPosX, kBestTimeBoxPosY, m_size.w, kBestTimeBoxPosY + kBestTimeBoxHeight, kBackFrameColor, true);
	DrawStringToHandle(m_size.w - kBestTimeUpStrPosX, kBestTimeUpStrPosY, L"ベストタイム更新！", kYellowColor, m_mgr.GetFont()->GetHandle(48));
}

/// <summary>
/// 条件達成の描画
/// </summary>
/// <param name="y">描画Y座標</param>
void StageBase::DrawConditionsAchived(int y) const
{
	// 送られてきた座標をずらす
	y += kAchivedStrShiftPosY;
	int backFrameY = y - kAchivedFrameDiff;

	for (const auto& achived : m_achived)
	{
		if (achived.frame < static_cast<int>(kAchivedDrawFrame * 0.5f))
		{
			DrawBox(0, backFrameY, kAchivedFrameWidth, backFrameY + kAchivedFrameHeight, kBackFrameColor, true);
			DrawStringToHandle(kAchivedStrPosX, y, achived.str.c_str(), kRedColor, m_mgr.GetFont()->GetHandle(64));
		}
		// 少しずつ透明に描画
		else
		{
			float rate = (kAchivedDrawFrame - achived.frame) / (kAchivedDrawFrame * 0.5f);
			int alpha = static_cast<int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawBox(0, backFrameY, kAchivedFrameWidth, backFrameY + kAchivedFrameHeight, kBackFrameColor, true);
			DrawStringToHandle(kAchivedStrPosX, y, achived.str.c_str(), kRedColor, m_mgr.GetFont()->GetHandle(64));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// 描画位置の更新
		y += kAchivedStrAdd;
		backFrameY += kAchivedStrAdd + kAchivedFrameDiff;
	}
}

/// <summary>
/// 条件の拡大描画
/// </summary>
void StageBase::DrawExpansion() const
{
	int width = m_size.w;
	int height = m_size.h;

	int posX = 0;

	int left = 0;
	int right = width + static_cast<int>((width - kExtShiftRight) * kExtRateSize);
	int top = static_cast<int>(-kExtShiftUp * kExtRateSize);
	int bottom = height + static_cast<int>((height - kExtShiftUp) * kExtRateSize);

	if (m_extRateFrame > kWaitExtRateFrame)
	{
		float rate = 1.0f - ((m_extRateFrame - kWaitExtRateFrame) / static_cast<float>(kExtRateFrame - kWaitExtRateFrame));

		posX = static_cast<int>(kExtShiftWidth * rate);

		right = width + static_cast<int>((width - kExtShiftRight) * kExtRateSize * rate);
		top = static_cast<int>(-kExtShiftUp * kExtRateSize * rate);
		bottom = height + static_cast<int>((height - kExtShiftUp) * kExtRateSize * rate);

		int alpha = kExtRateAlpha + static_cast<int>((255 - kExtRateAlpha) * (1.0f - rate));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}
	else
	{
		posX = kExtShiftWidth;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kExtRateAlpha);
	}

	DrawExtendGraph(posX + left, top, posX + right, bottom, m_extScreen, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// 壁の描画
/// </summary>
void StageBase::DrawWall() const
{
	int centerX = static_cast<int>(m_size.w * 0.5f);
	int centerY = static_cast<int>(m_size.h * 0.5f);

	// 画像中心を元にした描画をするために
	// Rotaにしている
	DrawRotaGraph(centerX, centerY, 1.0, 0.0, m_fieldImg->GetHandle(), true);
}

/// <summary>
/// 選択等の文字・画像描画
/// </summary>
/// /// <param name="cmd">コマンド名</param>
/// <param name="str">ウェーブさせる文字列</param>
/// <param name="num">文字列数</param>
void StageBase::DrawWave(const char* const cmd, const wchar_t* const str[], int num) const
{
	if (!m_isWaveDraw) return;

	DrawGraph(kWaveFramePosX, kWaveFramePosY, m_frameImg->GetHandle(), true);

	// 事前に入力したタイプで画像変更
	const auto& type = m_input.GetType();
	if (type == InputType::keybd)
	{
		m_key->DrawKey(m_input.GetHardDataName(cmd, InputType::keybd), kWaveImgPosX, kWaveImgPosY, 2.0);
	}
	else if (type == InputType::pad)
	{
		m_bt->DrawBottan(m_input.GetHardDataName(cmd, InputType::pad), kWaveImgPosX, kWaveImgPosY, 2.0);
	}

	int handle = m_mgr.GetFont()->GetHandle(32);

	int x = kWaveStrPosX;

	// 文字一つずつ描画
	for (int i = 0; i < num; i++)
	{
		int add = static_cast<int>(sinf(m_waveAngle + kWaveInterval * i) * -10);

		if (add > 0)
		{
			add = 0;
		}

		int y = kWaveStrPosY + add;


		DrawStringToHandle(x, y, str[i], kWhiteColor, handle);
		x += kWaveStrAdd;
	}
}

/// <summary>
/// 矢印の上に鍵の描画
/// </summary>
void StageBase::DrawArrowLock(int x, int y, bool isBossStage, bool isClear) const
{
	if (!isBossStage || isClear) return;

	DrawRotaGraph(x, y, 1.0, 0.0, m_arrowLockImg->GetHandle(), true);
}

/// <summary>
/// 矢印の画像をどれにするか決める
/// </summary>
/// <param name="isAlreadyClear">既にステージをクリアしたことがあるか</param>
/// <param name="nextStName">次のステージ名</param>
/// <returns>対応した画像ハンドル</returns>
int StageBase::GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle;

	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kArrowFlashInterval) % 2 == 0)
		{
			handle = m_arrowImg->GetHandle();
		}
		else
		{
			handle = m_arrowFlashImg->GetHandle();
		}
	}
	else
	{
		handle = m_arrowNoImg->GetHandle();
	}

	return handle;
}

void StageBase::LoadStageInfo()
{
	// 一時保存用string
	std::string strBuf;
	// カンマ分け一時保存用string
	std::vector<std::string> strConmaBuf;

	// ファイル読み込み
	std::ifstream ifs("StageData.csv");
	if (!ifs)
	{
		assert(false);
		return;
	}

	// ステージ名保存
	std::string stageName;
	// 敵種類数情報
	int enemyTypeIndex = 0;
	bool isLoadAllEnemys = true;
	// 敵1体の情報
	int enemyInfoIndex = 0;
	bool isLoadAllEnmeyInfo = true;
	// 隣接ステージ情報
	int nextStageIndex = 0;
	bool isLoadAllNextStages = true;

	// 最初は対応表情報が入っているだけなので無視する
	std::getline(ifs, strBuf);
	// 情報を読み切るまでループ
	while (std::getline(ifs, strBuf))
	{
		strConmaBuf = StringUtility::Split(strBuf, ',');

		LoadImportantStageInfo(strConmaBuf, stageName, isLoadAllEnemys, enemyTypeIndex, isLoadAllNextStages, nextStageIndex);
		auto& data = m_stageData[stageName];
		LoadEnemys(strConmaBuf, data, isLoadAllEnemys, enemyTypeIndex, isLoadAllEnmeyInfo, enemyInfoIndex);
		LoadNextStages(strConmaBuf, data, isLoadAllNextStages, nextStageIndex);
	}
}

void StageBase::LoadImportantStageInfo(std::vector<std::string>& strConmaBuf, std::string& stageName, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllNextStages, int& nextStageIndex)
{
	// 全ての情報を読み込んでいる場合のみ次の情報群に移行する
	if (!isLoadAllEnemys || !isLoadAllNextStages) return;

	// ステージ名読み込み
	stageName = strConmaBuf[kIndexStageName];
	// 更新タイプ読み込み
	UpTimeType timeType = static_cast<UpTimeType>(std::stoi(strConmaBuf[kIndexUpdateTimeType]));
	// 更新追加フレーム読み込み
	int addTimeFrame = std::stoi(strConmaBuf[kIndexAddTimeFrame]);
	// 敵種類数読み込み
	int enemyTypeNum = std::stoi(strConmaBuf[kIndexEnemyTypeNum]);
	// 隣接ステージ数読み込み
	int nextStageNum = std::stoi(strConmaBuf[kIndexNextStageNum]);
	// ボス生成フラグ読み込み
	bool isCreateBoss = static_cast<bool>(std::stoi(strConmaBuf[kIndexIsCreateBoss]));

	// 情報の代入
	auto& data = m_stageData[stageName];
	data.timeType = timeType;
	data.addTimeFrame = addTimeFrame;
	data.enemyNum = enemyTypeNum;
	data.enemyInfo.resize(enemyTypeNum);
	data.nextNum = nextStageNum;
	data.stageInfo.resize(nextStageNum);
	data.isBoss = isCreateBoss;

	// 情報読み込んでいないとする
	isLoadAllEnemys = false;
	enemyTypeIndex = 0;
	isLoadAllNextStages = false;
	nextStageIndex = 0;
}

void StageBase::LoadEnemys(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllEnemys, int& enemyTypeIndex, bool& isLoadAllEnmeyInfo, int& enemyInfoIndex)
{
	// 敵情報すべて読み込んでいたら早期リターン
	if (isLoadAllEnemys) return;

	auto& enemy = data.enemyInfo[enemyTypeIndex];

	if (isLoadAllEnmeyInfo)
	{
		// 名前読み込み
		std::string enemyName = strConmaBuf[kIndexEnemyName];
		// 同名別条件数読み込み
		int enemyInfoNum = std::stoi(strConmaBuf[kIndexEnemyInfoNum]);

		// 情報代入
		enemy.name = enemyName;
		enemy.num = enemyInfoNum;
		enemy.info.resize(enemyInfoNum);

		// 情報読み込み初期化
		isLoadAllEnmeyInfo = false;
		enemyInfoIndex = 0;
	}

	auto& info = enemy.info[enemyInfoIndex];

	// 初期生成数読み込み
	int startNum = std::stoi(strConmaBuf[kIndexStartCreateNum]);
	// 初期生成間隔読み込み
	int startCreateFrame = std::stoi(strConmaBuf[kIndexStartCreateFrame]);
	// 初期遅延フレーム
	int startDelayFrame = std::stoi(strConmaBuf[kIndexStartDelayFrame]);
	// 生成間隔
	int CreateFrame = std::stoi(strConmaBuf[kIndexCreateFrame]);

	// 情報代入
	info.startNum = startNum;
	info.startInterval = startCreateFrame;
	info.startDelayFrame = startDelayFrame;
	info.createInterval = CreateFrame;

	// 情報更新
	enemyInfoIndex++;
	if (enemyInfoIndex >= enemy.num)
	{
		isLoadAllEnmeyInfo = true;

		enemyTypeIndex++;
		if (enemyTypeIndex >= data.enemyNum)
		{
			isLoadAllEnemys = true;
		}
	}
}

void StageBase::LoadNextStages(std::vector<std::string>& strConmaBuf, StageData& data, bool& isLoadAllNextStages, int& nextStageIndex)
{
	// 隣接情報すべて読み込んでいたら早期リターン
	if (isLoadAllNextStages) return;

	// ステージ名読み込み
	std::string name = strConmaBuf[kIndexNextStageName];
	// 方向読み込み
	int dir = std::stoi(strConmaBuf[kIndexDir]);
	// 条件タイプ読み込み
	int infoType = std::stoi(strConmaBuf[kIndexInfoType]);
	// 条件情報読み込み
	int info = std::stoi(strConmaBuf[kIndexInfo]);

	// 情報代入
	auto& stage = data.stageInfo[nextStageIndex];
	stage.name = name;
	stage.dir = static_cast<MapDir>(dir);
	stage.type = static_cast<ConditionsType>(infoType);
	stage.info = info;

	// 合計時間系のみ追加情報読み込み
	if (static_cast<ConditionsType>(infoType) == ConditionsType::kSumTime)
	{
		// 情報群数読み込み
		int infoNum = std::stoi(strConmaBuf[kIndexInfoGroupNum]);
		stage.infoGroup.resize(infoNum);

		// 情報群読み込み
		for (int i = 0; i < infoNum; i++)
		{
			stage.infoGroup[i] = strConmaBuf[i + kIndexInfoGroup];
		}
	}

	// 情報更新
	nextStageIndex++;
	if (nextStageIndex >= data.nextNum)
	{
		isLoadAllNextStages = true;
	}
}

bool StageBase::UpdateTutorial()
{
	// チュートリアルステージでなければ早期リターン
	if (m_stageName != "練習") return false;

	// 説明総数を超えていたら早期リターン
	if (m_explanationIndex >= kExplanationNum) return false;

	m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_explanation[m_explanationIndex]->GetHandle()));
	m_explanationIndex++;
	m_isWaveDraw = false;

	return true;
}

void StageBase::DrawTutrial()
{
	if (m_emphasisFrame > kEmphasisFrame) return;

	if (m_emphasisFrame > kStartAlphaEmphasisFrame)
	{
		int alpha = static_cast<int>(255 * (1.0f - (m_emphasisFrame - kStartAlphaEmphasisFrame) / static_cast<float>(kEmphasisFrame - kStartAlphaEmphasisFrame)));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}

	float addX = sinf(m_emphasisFrame * kEmphasisAngle) * 5;

	int x = static_cast<int>(m_player->GetPos().x + 64 + addX);
	int y = static_cast<int>(m_player->GetPos().y);

	int index = (m_emphasisFrame / kEmphasisInterval) % 2;

	DrawRotaGraph(x, y, 1.0, 0.0, m_emphasisArrow[index]->GetHandle(), true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// ステージの初期化処理
/// </summary>
void StageBase::Init()
{
	auto& data = m_stageData[m_stageName];

	// 敵の総数を確認
	m_enemyNum = data.enemyNum;
	int temp = 0;
	for (int i = 0; i < m_enemyNum;i ++)
	{
		for (int j = 1; j < data.enemyInfo[i].num; j++)
		{
			temp++;
		}
	}
	m_enemyNum += temp;
	// サイズ更新
	m_enemyStarCreateNum.resize(m_enemyNum);
	m_enemyCreateFrame.resize(m_enemyNum);

	// クリア情報更新
	CheckStageConditions(m_mgr.GetStage()->GetBestTime(m_stageName));
	// 進行方向のクリアチェック
	for (const auto& stage : data.stageInfo)
	{
		bool isCreal = m_mgr.GetStage()->IsClearStage(stage.name);

		m_isClear[stage.dir] = isCreal;
	}

	// BGM変更
	auto& file = m_mgr.GetFile();
	if (m_stageData[m_stageName].isBoss)
	{
		m_playBgm = file->LoadSound(L"Bgm/boss.mp3");
	}
	else
	{
		m_playBgm = file->LoadSound(L"Bgm/fieldFight.mp3");
	}
}

void StageBase::PlayStart()
{
	// ゲーム動作側初期化
	m_emphasisFrame = 0;
	m_timeFrame = 0;
	m_waitFrame = 0;
	m_extRateFrame = 0;
	m_isUpdateBestTime = false;
	m_waveAngle = 0;
	m_achived.clear();

	// キャラ初期化
	m_player->Init();
	m_enemy.clear();
	m_backEnemy.clear();
	m_frontEnemy.clear();
	m_boss = nullptr;

	if (m_stageData[m_stageName].isBoss)
	{
		m_boss = std::make_shared<BossArmored>(m_size, m_fieldSize, this);
		m_boss->Init(m_centerPos);

		// ボスステージに入ったことがなければ説明
		if (!m_mgr.GetStage()->IsBossIn())
		{
			// 先にBGMをならして置かないと選択画面の音楽が流れるため流しておく
			m_soundSys->PlayFadeBgm(m_playBgm->GetHandle(), 0.8f);
			m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_explanation[kIndexBossExplanation]->GetHandle()));
			m_mgr.GetStage()->BossStageIn();
		}
	}
}

void StageBase::CreateEnemyType(const std::string& name, int& frame, bool isStart)
{
	if (name == "Normal")
	{
		CreateNormal(frame, isStart);
	}
	else if (name == "MoveWall")
	{
		CreateMoveWall();
	}
	else if (name == "Large")
	{
		CreateLarge(frame, isStart);
	}
	else if (name == "Dash")
	{
		CreateDash(frame, isStart);
	}
	else if (name == "Create")
	{
		CreateEneCreate(frame, isStart);
	}
	else if (name == "Division")
	{
		CreateDivision(frame, isStart);
	}
}

void StageBase::CreateEnemy()
{
	auto& data = m_stageData[m_stageName];

	int index = 0;
	// 敵種類全部回す
	for (int i = 0; i < data.enemyNum; i++)
	{
		auto& enemy = data.enemyInfo[i];

		// その種類のすべてのパターンを回す
		for (const auto& info : enemy.info)
		{
			auto& startNum = m_enemyStarCreateNum[index];
			auto& createFrame = m_enemyCreateFrame[index];

			createFrame++;
			// 初回生成
			if (startNum < info.startNum && createFrame > info.startInterval)
			{
				CreateEnemyType(enemy.name, createFrame, true);
				createFrame = info.startDelayFrame;

				startNum++;
			}
			// 通常生成  生成間隔が０より大きいことが条件
			else if (createFrame > info.createInterval && info.createInterval > 0)
			{
				CreateEnemyType(enemy.name, createFrame);
			}

			// 要素番号更新
			index++;
		}
	}
}

void StageBase::CheckStageConditions(int frame)
{
	for (const auto& stage : m_stageData[m_stageName].stageInfo)
	{
		auto& type = stage.type;
		if (type == ConditionsType::kTime)
		{
			CheckConditionsTime(stage.name, frame, stage.info, GetDirName(stage.dir));
		}
		else if (type == ConditionsType::kKilled)
		{
			CheckConditionsKilled(stage.name, stage.info, GetDirName(stage.dir));
		}
		else if (type == ConditionsType::kSumTime)
		{
			CheckConditionsSumTime(stage.name, stage.infoGroup, frame, stage.info, GetDirName(stage.dir));
		}
	}
}

std::wstring StageBase::GetDirName(MapDir dir)
{
	if (dir == MapDir::kUp)
	{
		return L"上";
	}
	else if (dir == MapDir::kDown)
	{
		return L"下";
	}
	else if (dir == MapDir::kRight)
	{
		return L"右";
	}
	else if (dir == MapDir::kLeft)
	{
		return L"左";
	}

	return L"";
}

int StageBase::DrawStageConditions(int drawY) const
{
	int startY = drawY;
	int fontHandle = m_mgr.GetFont()->GetHandle(28);

	for (const auto& stage : m_stageData.at(m_stageName).stageInfo)
	{
		// 既にクリアしていたら次へ
		if (m_isClear.at(stage.dir)) continue;

		// 矢印描画
		if (stage.dir == MapDir::kUp)
		{
			DrawArrowConditions(stage.name, drawY, 0.0);
		}
		else if (stage.dir == MapDir::kDown)
		{
			DrawArrowConditions(stage.name, drawY, DX_PI);
		}
		else if (stage.dir == MapDir::kRight)
		{
			DrawArrowConditions(stage.name, drawY, kRad90);
		}
		else if (stage.dir == MapDir::kLeft)
		{
			DrawArrowConditions(stage.name, drawY, -kRad90);
		}

		// 条件描画
		auto& type = stage.type;
		if (type == ConditionsType::kTime)
		{
			DrawTimeConditions(drawY, fontHandle, stage.info);
		}
		else if (type == ConditionsType::kKilled)
		{
			DrawKilledConditions(drawY, fontHandle, stage.info);
		}
		else if (type == ConditionsType::kSumTime)
		{
			DrawSumTimeConditions(stage.infoGroup, drawY, fontHandle, stage.info);
		}

		drawY += 68;
	}

	return drawY - startY - 68;
}

void StageBase::DrawArrow() const
{
	for (const auto& stage : m_stageData.at(m_stageName).stageInfo)
	{
		// 矢印描画
		if (stage.dir == MapDir::kUp)
		{
			DrawUpArrow(m_isClear.at(stage.dir), stage.name);
		}
		else if (stage.dir == MapDir::kDown)
		{
			DrawDownArrow(m_isClear.at(stage.dir), stage.name);
		}
		else if (stage.dir == MapDir::kRight)
		{
			DrawRightArrow(m_isClear.at(stage.dir), stage.name);
		}
		else if (stage.dir == MapDir::kLeft)
		{
			DrawLeftArrow(m_isClear.at(stage.dir), stage.name);
		}
	}
}

void StageBase::DrawEnemyKilledInfo(int x, int y) const
{
	int addX = 0;
	const auto& data = m_stageData.at(m_stageName);
	int bossNum = 0;

	for (const auto& enemy : data.enemyInfo)
	{
		DrawKilledEnemy(enemy.name, x, y, addX);

		addX += 36;

		if (enemy.name == "Create")
		{
			addX += 2;
			DrawKilledEnemy("Child", x, y, addX, 12);
			addX += 36 + 2;
		}
		else if (enemy.name == "Division")
		{
			addX += 2;
			DrawKilledEnemy("Split", x, y, addX, 12);
			addX += 36 + 2;
		}
		else if (enemy.name == "Large")
		{
			addX += 4;
		}
		else if (data.isBoss)
		{
			addX += 6;
			DrawKilledEnemy("BossArmored", x, y, addX);
			addX += 36 + 6;
			DrawKilledEnemy("BossStrongArmored", x, y, addX);
			addX += 36 + 6;
			DrawKilledEnemy("SplitTwoBound", x, y, addX, 12);
			addX += 36 + 2;
		}
	}
}

void StageBase::UpdateTime()
{
	auto& data = m_stageData[m_stageName];

	if (data.timeType == UpTimeType::kNormal)
	{
		UpTime();
	}
	else if (data.timeType == UpTimeType::kAttack)
	{
		// 弾を拾った時なのでここでは特に処理はしない
	}
}

void StageBase::UpTime()
{
	m_timeFrame += m_stageData[m_stageName].addTimeFrame;
}

void StageBase::CreateStrongBoss()
{
	std::shared_ptr<BossStrongArmored> strong;
	strong = std::make_shared<BossStrongArmored>(m_size, m_fieldSize, this);
	strong->Init(m_boss->GetPos());

	m_boss = strong;
}