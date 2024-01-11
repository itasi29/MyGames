#include <DxLib.h>
#include "Application.h"
#include "Input.h"
#include "StringUtility.h"

#include "StageBase.h"
#include "GameManager.h"
#include "StageManager.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"
#include "Boss/BossBase.h"

namespace
{
	// 矢印の点滅間隔
	constexpr int kFlashInterval = 20;
}

StageBase::StageBase(GameManager& mgr, float fieldSize) :
	m_mgr(mgr),
	m_size(Application::GetInstance().GetWindowSize()),
	m_fieldSize(fieldSize),
	m_centerPos({m_size.w * 0.5f, m_size.h * 0.5f}),
	m_frame(0),
	m_waitFrame(kWaitChangeFrame),
	m_isUpdateTime(false)
{
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;
}

StageBase::~StageBase()
{
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
	// 敵側だけ動く処理繰り返す
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

		// 各種初期化処理
		Init();
	}

	m_mgr.GetStage()->ChangeAbility(kDash);

	// フレームの増加
	m_waitFrame++;
}

void StageBase::UpdatePlaying(Input& input)
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

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}

	auto name = StringUtility::StringToWString(m_stageName);
	// ステージ名の描画
	DrawFormatString(128, 16, 0xffffff, L"%s", name.c_str());
	// 殺されたことがある敵の描画
	DrawKilledEnemyType();
	// 時間の描画
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(128, 32, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);
	// ステージ条件の描画
	DrawStageConditions();

	// ベストタイムの描画
	int bestTime = m_mgr.GetStage()->GetBestTime(m_stageName);
	minSec = (bestTime * 1000 / 60) % 1000;
	sec = (bestTime / 60) % 60;
	min = bestTime / 3600;
	DrawExtendString(m_size.w - 256, 32, 1.5, 1.5, L"ベストタイム", 0xffffff);
	DrawExtendFormatString(m_size.w - 256, 32 + 48, 2, 2, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);

	// 矢印の描画
	DrawArrow();
}

void StageBase::DrawPlaying()
{
	DrawWall();

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}
	if (m_boss)
	{
		m_boss->Draw();
	}

	// 時間の描画
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawExtendFormatString(128, 32,	// 表示位置
		2, 2,	// 拡大率
		0xffffff, // 色
		L"%02d:%02d.%03d", min, sec, minSec);	// 文字列
	// 条件の描画
	DrawStageConditions(64);
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
	ChangeClearData(StageManager::kStageRight, nextStage);

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
	ChangeClearData(StageManager::kStageLeft, nextStage);

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
	ChangeClearData(StageManager::kStageDown, nextStage);

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
	ChangeClearData(StageManager::kStageUp, nextStage);

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

void StageBase::BossDeath()
{
	// すでにクリアされていた場合は強化ボスを出す
	if (m_mgr.GetStage()->IsClearBoss(m_boss->GetName()))
	{
		CreateStrongBoss();
		return;
	}


	m_mgr.GetStage()->m_clear = true;

	// 初回殺しの時は倒したら終了とする
	// FIXME : 現状プレイヤーの死亡処理と同じにしているけれど後で処理の仕方変わると思う
	m_player->Death();

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

void StageBase::ChangeClearData(int dir, const std::shared_ptr<StageBase>& nextStage) const
{
	m_mgr.GetStage()->SaveClear(nextStage->GetStageName());
	nextStage->StartCheck();
}
