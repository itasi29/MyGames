﻿#include <DxLib.h>
#include <cassert>
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

namespace
{
	// フィールドサイズの倍率　(半分の大きさ)
	constexpr float kSizeScale = 0.4f;

	// 色定数
	constexpr unsigned int kFrameColor = 0xd80032;
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	constexpr unsigned int kYellowColor = 0xffde00;
	constexpr unsigned int kRedColor = 0xd2001a;
	constexpr unsigned int kBackFrameColor = 0x161a30;

	// 矢印の点滅間隔
	constexpr int kFlashInterval = 20;


	// 条件の描画基準位置
	constexpr int kConditionStrPosX = 20;

	// 殺された種類の基準描画位置
	constexpr int kKillTypePosX = 156;
	constexpr int kKillTypePosY = 200;
	// デフォルト拡大率
	constexpr double kKillTypeDefExtRate = 0.372;
	// 大きめ拡大率
	constexpr double kKillTypeLargeExtRate = 0.6;
	// それに対応する名前
	const std::vector<std::string> kLargeTypeName = {
		"Child",
		"Split",
		"SplitTwoBound"
	};
	// 小さめ拡大率
	constexpr double kKillTypeSmallExtRate = 0.25;
	// それに対応する名前
	const std::vector<std::string> kSmallTypeName = {
		"Large",
		"BossArmored",
		"BossStrongArmored"
	};

	// プレイヤー死亡時の画面の揺れフレーム
	constexpr int kShakeFrameDeath = 10;

	// サウンドのフェードフレーム
	constexpr int kSoundFade = 30;

	// ラジアンでの90度
	constexpr double kRad90 = DX_PI / 2;

	// 条件達成時の描画時間("○の条件達成の文字")
	constexpr int kAchivedFrame = 120;

	// スタート文字のウェーブスピード
	constexpr float kWaveSpeed = DX_PI_F / 180 * 5;
	// ウェーブの間隔
	constexpr float kWaveInterval = DX_PI_F / 15.0f;

	// ウェーブ文字列
	int kTitleWaveNum = 4;
	const wchar_t* const kTitleWave[] = {L"ス", L"タ", L"ー", L"ト"};
	int kDashWaveNum = 4;
	const wchar_t* const kDashWave[] = {L"ダ", L"ッ", L"シ", L"ュ"};

	// 拡大フレーム
	constexpr int kExtRateFrame = 45;
	// 拡大したままのフレーム
	constexpr int kWaitExtRateFrame = 30;
	// 拡大サイズ
	constexpr float kExtRateSize = 2.0f;
	// 
	constexpr int kShiftWidth = 256;
	// 右にずらす量
	constexpr int kShiftRight = 320;
	// 上にずらす量
	constexpr int kShiftUp = 360;
	// はじめのα値
	constexpr int kExtRateAlpha = 224;

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
	// 情報定数
	constexpr int kInfoFramePosX = 155;
	constexpr int kInfoFramePosY = 168;
	constexpr int kInfoBoxPosX = 0;
	constexpr int kInfoBoxPosY = 198;
	constexpr int kInfoBoxWidth = 310;
	constexpr int kInfoBoxHeight = 30;
	constexpr int kInfoBoxPlayingDiff = 6;

	// 条件定数
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
	constexpr int kBestTimeFrameSizeW = 155;
	constexpr int kBestTimeFrameSizeH = 96;
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
	m_waveAngle(DX_PI_F)
{
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;

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
	// BGM
	m_selectBgm = file->LoadSound(L"Bgm/provisionalBgm.mp3");
	m_playBgm = file->LoadSound(L"Bgm/fieldFight.mp3");
	// SE
	m_clearSe = file->LoadSound(L"Se/clear.mp3");
	// キー・ボタン
	m_bt = std::make_shared<BottansFile>(file);
	m_key = std::make_shared<KeyFile>(file);
}

StageBase::~StageBase()
{
	DeleteGraph(m_strScreen);
	DeleteGraph(m_extScreen);
}

void StageBase::Update(Input& input)
{
	// ウェーブ文字更新
	m_isWaveDraw = true;
	m_waveAngle -= kWaveSpeed;
	(this->*m_updateFunc)(input);
}

void StageBase::Draw() const
{
	(this->*m_drawFunc)();
}

void StageBase::Init()
{
	m_timeFrame = 0;
	m_waitFrame = 0;
	m_extRateFrame = 0;

	m_isUpdateBestTime = false;

	m_waveAngle = 0;

	m_player->Init();
	m_enemy.clear();
	m_backEnemy.clear();
	m_frontEnemy.clear();

	m_achived.clear();
}

void StageBase::GenericEnemy(const std::shared_ptr<EnemyBase>& enemy)
{
	m_backEnemy.push_back(enemy);
}

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
			return achived.frame > kAchivedFrame;
		}
	);

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

	// アビリティ変更
	m_mgr.GetStage()->ChangeAbility(Ability::kDash);
}

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
			BossDeath();
		}
		else if (!playerIsDash && playerCol.IsCollsion(m_boss->GetRect()))
		{
			m_player->Death();

			m_mgr.UpdateDeathCcount();
			m_mgr.GetStage()->UpdateEnemyType(m_boss->GetName());
			m_mgr.GetScene()->ShakeScreen(kShakeFrameDeath);
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
		m_soundSys->PlayFadeBgm(m_selectBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));

		m_waitFrame = 0;
		m_waveAngle = 0.0;
		m_isExtRate = false;

		// ベストタイムの更新
		if (m_mgr.GetStage()->UpdateBestTime(m_stageName, m_timeFrame))
		{
			m_isUpdateBestTime = true;
		}

		UniqueEndProcessing();

		m_updateFunc = &StageBase::UpdateSelect;
		m_drawFunc = &StageBase::DrawSelect;

		return;
	}
}

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

void StageBase::DrawAfterBossDeath() const
{
	DrawWall();

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	m_boss->Draw();
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

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

void StageBase::DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX, bool isReverxeY) const
{
	if (m_mgr.GetStage()->IsClearStage(nextStName) && (m_waitFrame / kFlashInterval) % 2 != 0)
	{
		DrawBox(kConditionStrPosX, y, kConditionStrPosX + kArrowSize, y + kArrowSize, 0xffde00, true);
	}
	DrawRotaGraph(kConditionStrPosX + static_cast<int>(kArrowSize * 0.5), y + static_cast<int>(kArrowSize * 0.5), 1.0, angle, m_arrowConditionsImg->GetHandle(), true, isReverseX, isReverxeY);
}

void StageBase::DrawTimeConditions(int y, int handle, int existTime) const
{
	DrawStringToHandle(kConditionStrPosX, y, L"　　   秒間生き残る\n　　(           )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionStrPosX, y, kYellowColor, handle, L"　　%2d\n　　  %2d / %2d",
		existTime, m_mgr.GetStage()->GetBestTime(m_stageName) / 60, existTime);
}

void StageBase::DrawKilledConditions(int y, int handle, int killedNum) const
{
	DrawStringToHandle(kConditionStrPosX, y, L"　　   種類の敵に殺される\n　　(          )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionStrPosX, y, kYellowColor, handle, L"　　%2d\n　　  %2d / %2d",
		killedNum, m_mgr.GetStage()->GetEnemyTypeCount(), killedNum);
}

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

void StageBase::DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f - kArrowShiftPos);
	int y = static_cast<int>(m_size.h * 0.5f);
	DrawRotaGraph(x, y, 1.0, -kRad90, handle, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

void StageBase::DrawRightArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f + kArrowShiftPos);
	int y = static_cast<int>(m_size.h * 0.5f);
	// MEMO:何故かReverXをtrueにするとがびらないからしておいてる
	DrawRotaGraph(x, y, 1.0, kRad90, handle, true, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

void StageBase::DrawUpArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f);
	int y = static_cast<int>(m_size.h * 0.5f - kArrowShiftPos);
	DrawRotaGraph(x, y, 1.0, 0.0, handle, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

void StageBase::DrawDownArrow(bool isAlreadyClear, const std::string& nextStName, bool isBossStage, bool isClear) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	int x = static_cast<int>(m_size.w * 0.5f);
	int y = static_cast<int>(m_size.h * 0.5f + kArrowShiftPos);
	DrawRotaGraph(x, y, 1.0, 0.0, handle, true, false, true);
	DrawArrowLock(x, y, isBossStage, isClear);
}

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
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
		DrawRotaGraph(x + addX, y, enemyExtRate, 0.0, file->GetHandle(), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void StageBase::DrawExpansion() const
{
	int width = m_size.w;
	int height = m_size.h;

	int posX = 0;

	int left = 0;
	int right = width + static_cast<int>((width - kShiftRight) * kExtRateSize);
	int top = static_cast<int>(-kShiftUp * kExtRateSize);
	int bottom = height + static_cast<int>((height - kShiftUp) * kExtRateSize);

	if (m_extRateFrame > kWaitExtRateFrame)
	{
		float rate = 1.0f - ((m_extRateFrame - kWaitExtRateFrame) / static_cast<float>(kExtRateFrame - kWaitExtRateFrame));

		posX = static_cast<int>(kShiftWidth * rate);

		right = width + static_cast<int>((width - kShiftRight) * kExtRateSize * rate);
		top = static_cast<int>(-kShiftUp * kExtRateSize * rate);
		bottom = height + static_cast<int>((height - kShiftUp) * kExtRateSize * rate);

		int alpha = kExtRateAlpha + static_cast<int>((255 - kExtRateAlpha) * (1.0f - rate));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}
	else
	{
		posX = kShiftWidth;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kExtRateAlpha);
	}

	DrawExtendGraph(posX + left, top, posX + right, bottom, m_extScreen, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

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

			m_mgr.GetScene()->ShakeScreen(kShakeFrameDeath);

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

void StageBase::BossDeath()
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

	m_updateFunc = &StageBase::UpdateAfterBossDeath;
	m_drawFunc = &StageBase::DrawAfterBossDeath;
}

void StageBase::DrawTime(int x, int y, int handle) const
{
	DrawStringToHandle(x, y, L"> タイム", kWhiteColor, m_mgr.GetFont()->GetHandle(24));
	y += kTimeAddY;
	int minSec = (m_timeFrame * 1000 / kFrameToSec) % 1000;
	int sec = (m_timeFrame / kFrameToSec) % kFrameToSec;
	int min = m_timeFrame / (kFrameToSec * kFrameToSec);
	DrawFormatStringToHandle(x, y, kYellowColor, handle, L"%01d:%02d.%03d", min, sec, minSec);
}

void StageBase::DrawBestTime() const
{
	// フレーム描画
	DrawRotaGraph(m_size.w - kBestTimeFrameSubX, kBestTimeFramePosY, 1.0, 0.0, m_backFrameImg->GetHandle(), true, true, true);
	DrawBox(m_size.w - kBestTimeFrameSubX - kBestTimeFrameSizeW, kBestTimeFramePosY + static_cast<int>(kBestTimeFrameSizeH * 0.5f), 
		m_size.w, kBestTimeFramePosY + kBestTimeFrameSizeH, kBackFrameColor, true);
	// ベストタイムの描画
	int bestTime = m_mgr.GetStage()->GetBestTime(m_stageName);
	int minSec = (bestTime * 1000 / kFrameToSec) % 1000;
	int sec = (bestTime / kFrameToSec) % kFrameToSec;
	int min = bestTime / (kFrameToSec * kFrameToSec);
	DrawStringToHandle(m_size.w - kBestTimeStrSubX, kBestTimeStrPosY, L"> ベストタイム", kWhiteColor, m_mgr.GetFont()->GetHandle(32));
	DrawFormatStringToHandle(m_size.w - kBestTimeStrSubX, kBestTimeStrPosY + kBestTimeStrAddY, kYellowColor, m_mgr.GetFont()->GetHandle(64), L"%02d:%02d.%03d", min, sec, minSec);

	if (!m_isUpdateBestTime) return;

	if (((m_waitFrame / kBestTimeFlashInterval) % 2) == 1)
	{
		DrawFormatStringToHandle(m_size.w - kBestTimeStrSubX, kBestTimeStrPosY + kBestTimeStrAddY, kRedColor, m_mgr.GetFont()->GetHandle(64), L"%02d:%02d.%03d", min, sec, minSec);
	}
	
	if (m_waitFrame > kAchivedFrame) return;

	if (m_waitFrame > static_cast<int>(kAchivedFrame * 0.5f))
	{
		float rate = 1.0f - (m_waitFrame - (kAchivedFrame * 0.5f)) / (kAchivedFrame * 0.5f);
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

void StageBase::DrawConditionsAchived(int y) const
{
	// 送られてきた座標をずらす
	y += kAchivedStrShiftPosY;
	int backFrameY = y - kAchivedFrameDiff;

	for (const auto& achived : m_achived)
	{
		if (achived.frame < static_cast<int>(kAchivedFrame * 0.5f))
		{
			DrawBox(0, backFrameY, kAchivedFrameWidth, backFrameY + kAchivedFrameHeight, kBackFrameColor, true);
			DrawStringToHandle(kAchivedStrPosX, y, achived.str.c_str(), kRedColor, m_mgr.GetFont()->GetHandle(64));
		}
		else
		{
			float rate = (kAchivedFrame - achived.frame) / (kAchivedFrame * 0.5f);
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

void StageBase::DrawArrowLock(int x, int y, bool isBossStage, bool isClear) const
{
	if (!isBossStage || isClear) return;

	DrawRotaGraph(x, y, 1.0, 0.0, m_arrowLockImg->GetHandle(), true);
}

void StageBase::DrawWall() const
{
	int centerX = static_cast<int>(m_size.w * 0.5f);
	int centerY = static_cast<int>(m_size.h * 0.5f);

	// 画像中心を元にした描画をするために
	// Rotaにしている
	DrawRotaGraph(centerX, centerY, 1.0, 0.0, m_fieldImg->GetHandle(), true);
}

void StageBase::DrawWave(const char* const cmd, const wchar_t* const str[], int num) const
{
	if (!m_isWaveDraw) return;

	DrawGraph(kWaveFramePosX, kWaveFramePosY, m_frameImg->GetHandle(), true);

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

int StageBase::GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle;

	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
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
