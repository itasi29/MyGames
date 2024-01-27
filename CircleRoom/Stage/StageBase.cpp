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

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// 強調文字列の色
	constexpr unsigned int kYellowColor = 0xffde00;;
	// バックフレームの色
	constexpr unsigned int kBackFrameColor = 0x161a30;

	// 矢印の点滅間隔
	constexpr int kFlashInterval = 20;


	// 条件の描画基準位置
	constexpr int kConditionsPosX = 20;

	// 殺された種類の基準描画位置
	constexpr int kKillTypePosX = 156;
	constexpr int kKillTypePosY = 200;

	// プレイヤー死亡時の画面の揺れフレーム
	constexpr int kShakeFrameDeath = 10;

	// サウンドのフェードフレーム
	constexpr int kSoundFade = 30;

	// ラジアンでの90度
	constexpr double kRad90 = DX_PI / 2;

	// 条件達成時の描画時間("○の条件達成の文字")
	constexpr int kAchivedFrame = 120;
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
	m_field = file->LoadGraphic(L"field.png");
	m_arrow = file->LoadGraphic(L"UI/arrow.png");
	m_arrowFlash = file->LoadGraphic(L"UI/arrowFlash.png");
	m_arrowNo = file->LoadGraphic(L"UI/arrowNo.png");
	m_arrowConditions = file->LoadGraphic(L"UI/arrowConditions.png");
	m_startFrame = file->LoadGraphic(L"UI/startFrame.png");
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
	// 待機フレームの増加
	m_waitFrame++;

	if (m_waitFrame < kWaitChangeFrame) return;

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

		// メンバ関数ポインタの更新
		m_updateFunc = &StageBase::UpdatePlaying;
		m_drawFunc = &StageBase::DrawPlaying;

		m_sound->Stop(m_selectBgm->GetHandle());
		m_soundFrame = 0;

		// 各種初期化処理
		Init();
	}

	m_mgr.GetStage()->ChangeAbility(kDash);
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
	DrawBox(0, 28, 256, 104, kFrameColor, true);
	DrawFormatStringToHandle(64, 32, kYellowColor, m_mgr.GetFont()->GetHandle(64), L"%s", name.c_str());

	int fontHandle = m_mgr.GetFont()->GetHandle(32);
	int drawScreenHandle = m_mgr.GetScene()->GetScreenHandle();

	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	// 時間の描画
	DrawTime(20, 160, fontHandle);
	// 殺されたことがある敵の描画
	DrawStringToHandle(136, 160, L"> Circle", kWhiteColor, m_mgr.GetFont()->GetHandle(24));
	DrawKilledEnemyType();
	SetDrawScreen(drawScreenHandle);
	// 上下反転して描画フレーム
	DrawRotaGraph(155, 168, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawBox(0, 198, 310, 198 + 30, kBackFrameColor, true);
	DrawGraph(0, 0, m_strHandle, true);

	// ステージ条件の描画
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(256);
	SetDrawScreen(drawScreenHandle);
	// 条件後ろにあるフレーム背景を描画する
	if (y >= 0)
	{
		DrawBox(0, 244, 310, 244 + y, kBackFrameColor, true);
		DrawGraph(0, 244 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

	// フレーム描画
	DrawRotaGraph(m_size.w - 128, 128, 1.0, 0.0, m_bFrameImg->GetHandle(), true, true, true);
	DrawBox(m_size.w - 128 - 155, 158, m_size.w, 224, kBackFrameColor, true);
	// ベストタイムの描画
	int bestTime = m_mgr.GetStage()->GetBestTime(m_stageName);
	int minSec = (bestTime * 1000 / 60) % 1000;
	int sec = (bestTime / 60) % 60;
	int min = bestTime / 3600;
	DrawStringToHandle(m_size.w - 256, 112, L"> ベストタイム", kWhiteColor, m_mgr.GetFont()->GetHandle(32));
	DrawFormatStringToHandle(m_size.w - 256, 112 + 48, kYellowColor, m_mgr.GetFont()->GetHandle(64), L"%02d:%02d.%03d", min, sec, minSec);

	// 矢印の描画
	DrawArrow();

	// スタートの画像の描画
	DrawImage();

	// 条件達成時のを描画
	DrawConditionsAchived();
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

	int drawScreenHandle = m_mgr.GetScene()->GetScreenHandle();

	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	// 時間の描画
	DrawTime(20, 144, m_mgr.GetFont()->GetHandle(64));
	SetDrawScreen(drawScreenHandle);
	DrawRotaGraph(155, 168, 1.0, 0.0, m_bFrameImg->GetHandle(), true, false, true);
	DrawBox(0, 198, 310, 198 + 36, kBackFrameColor, true);
	DrawGraph(0, 0, m_strHandle, true);

	// 条件の描画
	SetDrawScreen(m_strHandle);
	ClearDrawScreen();
	auto y = DrawStageConditions(244+16+20);
	SetDrawScreen(drawScreenHandle);
	// MEMO:条件後ろにあるフレーム背景を描画する
	if (y >= 0)
	{
		DrawBox(0, 268, 311, 268 + y, kBackFrameColor, true);
		DrawGraph(0, 268 + y, m_bFrameImg->GetHandle(), true);
	}
	DrawGraph(0, 0, m_strHandle, true);

}

void StageBase::DrawArrowConditions(const std::string& nextStName, int y, double angle, bool isReverseX, bool isReverxeY)
{
	if (m_mgr.GetStage()->IsClearStage(nextStName) && (m_waitFrame / kFlashInterval) % 2 != 0)
	{
		DrawBox(kConditionsPosX, y, kConditionsPosX + 28, y + 28, 0xffde00, true);
	}
	DrawRotaGraph(kConditionsPosX + 14, y + 14, 1.0, angle, m_arrowConditions->GetHandle(), true, isReverseX, isReverxeY);
}

void StageBase::DrawTimeConditions(int y, int handle, int existTime)
{
	DrawStringToHandle(kConditionsPosX, y, L"　　   秒間生き残る\n　　(          )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionsPosX, y, kYellowColor, handle, L"　　%2d\n　　  %2d / %2d",
		existTime, m_mgr.GetStage()->GetBestTime(m_stageName) / 60, existTime);
}

void StageBase::DrawKilledConditions(int y, int handle, int killedNum)
{
	DrawStringToHandle(kConditionsPosX, y, L"　　   種類の敵に殺される\n　　(          )", kWhiteColor, handle);
	DrawFormatStringToHandle(kConditionsPosX, y, kYellowColor, handle, L"　　%2d\n　　  %2d / %2d",
		killedNum, m_mgr.GetStage()->GetEnemyTypeCount(), killedNum);
}

void StageBase::DrawLeftArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	DrawRotaGraph(static_cast<int>(m_size.w * 0.5f - 150), static_cast<int>(m_size.h * 0.5f), 1.0, -kRad90, handle, true);
}

void StageBase::DrawRightArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	// MEMO:何故かReverXをtrueにするとがびらないからしておいてる
	DrawRotaGraph(static_cast<int>(m_size.w * 0.5f + 150), static_cast<int>(m_size.h * 0.5f), 1.0, kRad90, handle, true, true);
}

void StageBase::DrawUpArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	DrawRotaGraph(static_cast<int>(m_size.w * 0.5f), static_cast<int>(m_size.h * 0.5f - 150), 1.0, 0.0, handle, true);
}

void StageBase::DrawDownArrow(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle = GetArrowHandle(isAlreadyClear, nextStName);

	DrawRotaGraph(static_cast<int>(m_size.w * 0.5f), static_cast<int>(m_size.h * 0.5f + 150), 1.0, 0.0, handle, true, false, true);
}

void StageBase::DrawKilledEnemy(const std::string& enemyName, int addX, unsigned int color, int radius) const
{
	if (m_mgr.GetStage()->IsKilledEnemy(enemyName))
	{
		DrawCircle(kKillTypePosX + addX, kKillTypePosY, radius, color, true);
	}
	else
	{
		DrawCircle(kKillTypePosX + addX, kKillTypePosY , radius, color, false);
	}
}

void StageBase::AddAchivedStr(const std::wstring& dir)
{
	m_achived.push_back({ dir + L"の条件達成！", 0 });
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

void StageBase::DrawTime(int x, int y, int handle)
{
	DrawStringToHandle(x, y, L"> タイム", kWhiteColor, m_mgr.GetFont()->GetHandle(24));
	y += 24;
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatStringToHandle(x, y, kYellowColor, handle, L"%01d:%02d.%03d", min, sec, minSec);
}

void StageBase::DrawConditionsAchived()
{
	int y = 180;
	for (const auto& achived : m_achived)
	{
		if (achived.frame < static_cast<int>(kAchivedFrame * 0.5f))
		{
			DrawStringToHandle(540, y, achived.str.c_str(), kYellowColor, m_mgr.GetFont()->GetHandle(64));
		}
		else
		{
			float rate = (kAchivedFrame - achived.frame) / (kAchivedFrame * 0.5f);
			int alpha = static_cast<int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(540, y, achived.str.c_str(), kYellowColor, m_mgr.GetFont()->GetHandle(64));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		y += 64;
	}
}

void StageBase::DrawWall()
{
	int centerX = static_cast<int>(m_size.w * 0.5f);
	int centerY = static_cast<int>(m_size.h * 0.5f);

	// 画像中心を元にした描画をするために
	// Rotaにしている
	DrawRotaGraph(centerX, centerY, 1.0, 0.0, m_field->GetHandle(), true);
}

void StageBase::DrawImage()
{

	DrawGraph(980, 595, m_startFrame->GetHandle(), true);
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
	DrawStringToHandle(1064, 600, L"スタート", kWhiteColor, m_mgr.GetFont()->GetHandle(32));
}

int StageBase::GetArrowHandle(bool isAlreadyClear, const std::string& nextStName) const
{
	int handle;

	if (m_mgr.GetStage()->IsClearStage(nextStName))
	{
		if (isAlreadyClear || (m_waitFrame / kFlashInterval) % 2 == 0)
		{
			handle = m_arrow->GetHandle();
		}
		else
		{
			handle = m_arrowFlash->GetHandle();
		}
	}
	else
	{
		handle = m_arrowNo->GetHandle();
	}

	return handle;
}
