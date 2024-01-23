#include <DxLib.h>
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
#include "Boss/BossBase.h"

namespace
{
	// フレームの色
	constexpr unsigned int kFrameColor = 0xd80032;

	// フィールドサイズの倍率
	// フィールドはwindowsizeの縦幅に倍率をかけたものとする
	constexpr float kSizeScale = 0.4f;

	// 文字列の色
	constexpr unsigned int kStrColor = 0xf0ece5;

	// 矢印の点滅間隔
	constexpr int kFlashInterval = 20;

	// プレイヤー死亡時の画面の揺れフレーム
	constexpr int kShakeFrameDeath = 10;

	// サウンドのフェードフレーム
	constexpr int kSoundFade = 30;
}

StageBase::StageBase(GameManager& mgr, Input& input) :
	m_mgr(mgr),
	m_input(input),
	m_size(Application::GetInstance().GetWindowSize()),
	m_fieldSize(m_size.h* kSizeScale),
	m_centerPos({m_size.w * 0.5f, m_size.h * 0.5f}),
	m_soundFrame(kSoundFade),
	m_frame(0),
	m_waitFrame(kWaitChangeFrame),
	m_isUpdateTime(false)
{
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;

	// 第三引数をtrueにしておかないと作った画面が透過しない
	m_strHandle = MakeScreen(m_size.w, m_size.h, true);

	m_sound = m_mgr.GetSound();

	auto& file = m_mgr.GetFile();
	m_bFrameImg = file->LoadGraphic(L"UI/backFrame.png");
	m_selectBgm = file->LoadSound(L"Bgm/provisionalBgm.mp3");
	m_playBgm = file->LoadSound(L"Bgm/fieldFight.mp3");

	m_bt = std::make_shared<BottansFile>(file);
	m_key = std::make_shared<KeyFile>(file);
}

StageBase::~StageBase()
{
	DeleteGraph(m_strHandle);
}

void StageBase::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void StageBase::Draw()
{
	(this->*m_drawFunc)();
}

void StageBase::GenericEnemy(const std::shared_ptr<EnemyBase>& enemy)
{
	m_enemy.push_back(enemy);
}

void StageBase::UpdateSelect(Input& input)
{
	if (m_soundFrame > kSoundFade)
	{
		m_sound->PlayBgm(m_selectBgm->GetHandle());
	}
	else
	{
		m_soundFrame++;
		m_sound->PlayFadeBgm(m_selectBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));
	}

	m_player->Update(input, kNone);

	for (const auto& enemy : m_enemy)
	{
		enemy->Update();
	}

	m_enemy.remove_if(
		[](const auto& enemy)
		{
			return !enemy->IsExsit();
		});
	if (m_boss)
	{
		m_boss->Update();
	}

	if (input.IsPress("OK"))
	{
		// メンバ関数ポインタの更新
		m_updateFunc = &StageBase::UpdatePlaying;
		m_drawFunc = &StageBase::DrawPlaying;

		m_sound->Stop(m_selectBgm->GetHandle());
		m_soundFrame = 0;

		// 各種初期化処理
		Init();
	}

	m_mgr.GetStage()->ChangeAbility(kDash);

	// フレームの増加
	m_waitFrame++;
}

void StageBase::UpdatePlaying(Input& input)
{
	if (m_soundFrame > kSoundFade)
	{
		m_sound->PlayBgm(m_playBgm->GetHandle());
	}
	else
	{
		m_soundFrame++;
		m_sound->PlayFadeBgm(m_playBgm->GetHandle(), m_soundFrame / static_cast<float>(kSoundFade));
	}

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
			m_mgr.UpdateDeathCcount();

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

	if (m_boss)
	{
		m_boss->Update();
		// ボスにダメージを与えたら時間を増やす
		m_isUpdateTime = m_boss->OnAttack(playerIsDash, playerCol);

		// ボスの死亡処理
		if (!m_boss->IsExsit())
		{
			// 初回のみクリア処理に移動する
			BossDeath();
		}
		// ボスが生存していれば
		// プレイヤーとの判定処理
		else if (!playerIsDash && playerCol.IsCollsion(m_boss->GetRect()))
		{
			// プレイヤーの死亡処理
			m_player->Death();
			m_mgr.GetScene()->ShakeScreen(kShakeFrameDeath);
			m_mgr.UpdateDeathCcount();

			// 殺したことがある敵情報の更新
			m_mgr.GetStage()->UpdateEnemyType(m_boss->GetName());
		}
	}

	UpdateTime();

	if (!m_player->IsExsit())
	{
		// メンバ関数ポインタを選択の方に戻す
		m_updateFunc = &StageBase::UpdateSelect;
		m_drawFunc = &StageBase::DrawSelect;

		// 音関係の設定
		m_soundFrame = 0;
		m_sound->Stop(m_playBgm->GetHandle());

		// フレームの初期化
		m_waitFrame = 0;

		// ベストタイムの更新
		m_mgr.GetStage()->UpdateBestTime(m_stageName, m_frame);

		// クリアしているかの確認
		CheckStageConditions();
	}
}

void StageBase::DrawSelect()
{
	DrawWall();

	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}
	m_player->Draw();
	auto name = StringUtility::StringToWString(m_stageName);
	// ステージ名の描画
	DrawFormatStringToHandle(128, 16, kStrColor, m_mgr.GetFont()->GetHandle(64), L"%s", name.c_str());

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	// 時間の描画
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatStringToHandle(128, 96, kStrColor, fontHandle, L"%02d:%02d.%03d", min, sec, minSec);
	// 殺されたことがある敵の描画
	DrawKilledEnemyType();
	SetDrawScreen(DX_SCREEN_BACK);
	// 上下反転して描画フレーム
	DrawRotaGraph(155, 120, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawBox(0, 120 + 30, 211 + 100, 120 + 30 + 30, 0xB6BBC4, true);
	DrawGraph(0, 0, m_strHandle, true);

	// ステージ条件の描画
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(196);
	SetDrawScreen(DX_SCREEN_BACK);
	// 条件後ろにあるフレーム背景を描画する
	if (y >= 0)
	{
		DrawBox(0, 196, 311, 196 + y, 0xb6bbc4, true);
		DrawGraph(0, 196 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

	// フレーム描画
	DrawRotaGraph(m_size.w - 128, 48, 1.0, 0.0, m_bFrameImg->GetHandle(), true, true, true);
	DrawBox(m_size.w - 128 - 156, 78, m_size.w, 144, 0xb6bbc4, true);
	// ベストタイムの描画
	int bestTime = m_mgr.GetStage()->GetBestTime(m_stageName);
	minSec = (bestTime * 1000 / 60) % 1000;
	sec = (bestTime / 60) % 60;
	min = bestTime / 3600;
	DrawExtendStringToHandle(m_size.w - 256, 32, 1.5, 1.5, L"ベストタイム", kStrColor, fontHandle);
	DrawExtendFormatStringToHandle(m_size.w - 256, 32 + 48, 2, 2, kStrColor, fontHandle, L"%02d:%02d.%03d", min, sec, minSec);

	// 矢印の描画
	DrawArrow();

	DrawImage();
}

void StageBase::DrawPlaying()
{
	DrawWall();

	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}
	m_player->Draw();

	// 時間の描画
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawExtendFormatString(128, 32,	// 表示位置
		2, 2,	// 拡大率
		kStrColor, // 色
		L"%02d:%02d.%03d", min, sec, minSec);	// 文字列
	SetDrawScreen(DX_SCREEN_BACK);
	DrawRotaGraph(155, 48, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawGraph(0, 0, m_strHandle, true);

	// 条件の描画
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(196+24);
	SetDrawScreen(DX_SCREEN_BACK);
	// MEMO:条件後ろにあるフレーム背景を描画する
	if (y >= 0)
	{
		DrawBox(0, 196 + 24, 311, 196 + 24 + y, 0xb6bbc4, true);
		DrawGraph(0, 196 + 24 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

}

void StageBase::DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	unsigned int color = 0;
	// クリアしている場合は濃いめで
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			color = 0xffffff;
		}
		else
		{
			color = 0xffff08;
		}
	}
	// クリアしていない場合は薄めで
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_size.w / 2 - 150, m_size.h / 2,
		m_size.w / 2 - 100, m_size.h / 2 + 25,
		m_size.w / 2 - 100, m_size.h / 2 - 25,
		color, true);
}

void StageBase::DrawRightArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	unsigned int color = 0;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			color = 0xffffff;
		}
		else
		{
			color = 0xffff08;
		}
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_size.w / 2 + 150, m_size.h / 2,
		m_size.w / 2 + 100, m_size.h / 2 + 25,
		m_size.w / 2 + 100, m_size.h / 2 - 25,
		color, true);
}

void StageBase::DrawUpArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	unsigned int color = 0;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			color = 0xffffff;
		}
		else
		{
			color = 0xffff08;
		}
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_size.w / 2, m_size.h / 2 - 150,
		m_size.w / 2 + 25, m_size.h / 2 - 100,
		m_size.w / 2 - 25, m_size.h / 2 - 100,
		color, true);
}

void StageBase::DrawDownArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	unsigned int color;
	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			color = 0xffffff;
		}
		else
		{
			color = 0xffff08;
		}
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_size.w / 2, m_size.h / 2 + 150,
		m_size.w / 2 + 25, m_size.h / 2 + 100,
		m_size.w / 2 - 25, m_size.h / 2 + 100,
		color, true);
}

void StageBase::SlideLeft(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:今からくそコード書くから後で直して
	// クリア情報の更新
	ChangeClearData(nextStage);

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// 送る用の描画先を作成する
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_size.w * 2, m_size.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	// 次のステージを下に動いていたらずらす
	DrawGraph(0, m_mgr.GetStage()->GetSlideVolumeY(StageManager::kStageDown), nextScreenHandle, true);
	// 現在の画像を上に動いていたらずらす
	DrawGraph(m_size.w, m_mgr.GetStage()->GetSlideVolumeY(StageManager::kStageUp), nowScreenHandle, true);

	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	// 送る方は送ったほうで消すため考えない
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr.GetStage()->StartMove(StageManager::kStageLeft, sendScreenHandle);

	// 次のステージに変更する
	m_mgr.GetStage()->ChangeStage(nextStage);
}

void StageBase::SlideRight(std::shared_ptr<StageBase> nextStage)
{
	// FIXEME: クソコード書くから後で直して
	// クリア情報の更新
	ChangeClearData(nextStage);

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_size.w * 2, m_size.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(0, m_mgr.GetStage()->GetSlideVolumeY(StageManager::kStageUp), nowScreenHandle, true);
	DrawGraph(m_size.w, m_mgr.GetStage()->GetSlideVolumeY(StageManager::kStageDown), nextScreenHandle, true);

	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);
	
	// 画面を動かす処理を実行する
	m_mgr.GetStage()->StartMove(StageManager::kStageRight, sendScreenHandle);

	// 次のステージに変更する
	m_mgr.GetStage()->ChangeStage(nextStage);
}

void StageBase::SlideUp(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:今からくそコード書くから後で直して
	// クリア情報の更新
	ChangeClearData(nextStage);

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);


	// 送る用の描画先を作成する
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_size.w * 2, m_size.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr.GetStage()->GetSlideVolumeX(StageManager::kStageRight), 0, nextScreenHandle, true);
	DrawGraph(m_mgr.GetStage()->GetSlideVolumeX(StageManager::kStageLeft), m_size.h, nowScreenHandle, true);
	
	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr.GetStage()->StartMove(StageManager::kStageUp, sendScreenHandle);

	// 次のステージに変更する
	m_mgr.GetStage()->ChangeStage(nextStage);
}

void StageBase::SlideDown(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:今からくそコード書くから後で直して
	// クリア情報の更新
	ChangeClearData(nextStage);

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// 送る用の描画先を作成する
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_size.w * 2, m_size.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr.GetStage()->GetSlideVolumeX(StageManager::kStageLeft), 0, nowScreenHandle, true);
	DrawGraph(m_mgr.GetStage()->GetSlideVolumeX(StageManager::kStageRight), m_size.h, nextScreenHandle, true);

	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr.GetStage()->StartMove(StageManager::kStageDown, sendScreenHandle);

	// 次のステージに変更する
	m_mgr.GetStage()->ChangeStage(nextStage);
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

void StageBase::BossDeath()
{
	// すでにクリアされていた場合は強化ボスを出す
	if (m_mgr.GetStage()->IsClearBoss(m_boss->GetName()))
	{
		CreateStrongBoss();
		return;
	}


	m_mgr.GetStage()->m_clear = true;

	// メンバ関数ポインタを選択の方に戻す
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;

	// 倒した情報の追加
	m_mgr.GetStage()->UpdateClearBoss(m_boss->GetName());

	// ボスを消す
	m_boss.reset();
	// 敵全て消す
	m_enemy.clear();
}

void StageBase::DrawWall()
{
	float centerX = m_size.w * 0.5f;
	float centerY = m_size.h * 0.5f;

	// 色は仮
	DrawBox(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0xffffff, false);
}

void StageBase::DrawImage()
{
	DrawBox(1000, 600, 1280, 632, kFrameColor, true);
	switch (m_input.GetType())
	{
	case InputType::keybd:
		m_key->DrawKey(m_input.GetHardDataName("OK", InputType::keybd), 1016, 600, 2.0);
		break;
	default:
		assert(false);
	case InputType::pad:
		m_bt->DrawBottan(m_input.GetHardDataName("OK", InputType::pad), 1016, 600, 2.0);
		break;
	}
	DrawStringToHandle(1064, 600, L"選択", kStrColor, m_mgr.GetFont()->GetHandle(32));
}

void StageBase::SlideStart(int& now, int& next, const std::shared_ptr<StageBase>& nextStage)
{
	// 現在の画面を保存するよう
	now = MakeScreen(m_size.w, m_size.h, true);
	// 描画先変更
	SetDrawScreen(now);
	// 現在の画面を描画
	Draw();
	// 次の画面を保持するよう
	next = MakeScreen(m_size.w, m_size.h, true);
	SetDrawScreen(next);
	nextStage->Draw();
}

void StageBase::ChangeClearData(const std::shared_ptr<StageBase>& nextStage) const
{
	m_mgr.GetStage()->SaveClear(nextStage->GetStageName());
	nextStage->StartCheck();
}
