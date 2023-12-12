#include <DxLib.h>
#include "Application.h"
#include "Input.h"
#include "StringUtility.h"

#include "StageBase.h"
#include "StageManager.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"
#include "Boss/BossBase.h"

StageBase::StageBase(StageManager& mgr, const Size& windowSize, float fieldSize) :
	m_centerPos( { windowSize.w * 0.5f, windowSize.h * 0.5f }),
	m_mgr(mgr),
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_frame(0),
	m_waitFrame(kWaitChangeFrame)
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

	// フレームの増加
	m_waitFrame++;
}

void StageBase::UpdatePlaying(Input& input)
{
	m_player->Update(input);

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

			// メンバ関数ポインタを選択の方に戻す
			m_updateFunc = &StageBase::UpdateSelect;
			m_drawFunc = &StageBase::DrawSelect;

			// フレームの初期化
			m_waitFrame = 0;

			// ベストタイムの更新
			m_mgr.UpdateBestTime(m_stageName, m_frame);
			// 殺したことがある敵情報の更新
			m_mgr.UpdateEnemyType(enemy->GetName());

			// クリアしているかの確認
			CheckStageConditions();

			return;
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
		m_boss->OnAttack(playerIsDash, playerCol);

		// ボスの死亡処理
		if (!m_boss->IsExsit())
		{
			// ボスを消す
			m_boss.reset();
			// 敵全て消す
			m_enemy.clear();

			// FIXME : 現状プレイヤーの死亡処理と同じにしているけれど後で処理の仕方変わると思う
			m_player->Death();

			// メンバ関数ポインタを選択の方に戻す
			m_updateFunc = &StageBase::UpdateSelect;
			m_drawFunc = &StageBase::DrawSelect;

			// フレームの初期化
			m_waitFrame = 0;

			// ベストタイムの更新
			m_mgr.UpdateBestTime(m_stageName, m_frame);

			// クリアしているかの確認
			CheckStageConditions();

			return;
		}

		// プレイヤーとの判定処理
		if (!playerIsDash && playerCol.IsCollsion(m_boss->GetRect()))
		{
			// プレイヤーの死亡処理
			m_player->Death();

			// メンバ関数ポインタを選択の方に戻す
			m_updateFunc = &StageBase::UpdateSelect;
			m_drawFunc = &StageBase::DrawSelect;

			// フレームの初期化
			m_waitFrame = 0;

			// ベストタイムの更新
			m_mgr.UpdateBestTime(m_stageName, m_frame);
			// 殺したことがある敵情報の更新
			m_mgr.UpdateEnemyType(m_boss->GetName());

			// クリアしているかの確認
			CheckStageConditions();

			return;
		}
	}

	// 経過時間の更新
	m_frame++;
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
	// 時間の描画
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(128, 32, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);
	// ステージ条件の描画
	DrawStageConditions();

	// ベストタイムの描画
	int bestTime = m_mgr.GetBestTime(m_stageName);
	minSec = (bestTime * 1000 / 60) % 1000;
	sec = (bestTime / 60) % 60;
	min = bestTime / 3600;
	DrawExtendString(m_windowSize.w - 256, 32, 1.5, 1.5, L"ベストタイム", 0xffffff);
	DrawExtendFormatString(m_windowSize.w - 256, 32 + 48, 2, 2, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);

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

void StageBase::DrawLeftArrow() const
{
	unsigned int color = 0;
	// クリアしている場合は濃いめで
	if (m_mgr.IsClear(m_stageName, StageManager::kStageLeft))
	{
		color = 0xffffff;
	}
	// クリアしていない場合は薄めで
	else
	{
		color = 0x808080;
	}
	DrawTriangle(100, m_windowSize.h / 2,
		150, m_windowSize.h / 2 + 25,
		150, m_windowSize.h / 2 - 25,
		color, true);
}

void StageBase::DrawRightArrow() const
{
	unsigned int color = 0;
	if (m_mgr.IsClear(m_stageName, StageManager::kStageRight))
	{
		color = 0xffffff;
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_windowSize.w - 100, m_windowSize.h / 2,
		m_windowSize.w - 150, m_windowSize.h / 2 + 25,
		m_windowSize.w - 150, m_windowSize.h / 2 - 25,
		color, true);
}

void StageBase::DrawUpArrow() const
{
	unsigned int color = 0;
	if (m_mgr.IsClear(m_stageName, StageManager::kStageUp))
	{
		color = 0xffffff;
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_windowSize.w / 2, 100,
		m_windowSize.w / 2 + 25, 150,
		m_windowSize.w / 2 - 25, 150,
		color, true);
}

void StageBase::DrawDownArrow() const
{
	unsigned int color;
	if (m_mgr.IsClear(m_stageName, StageManager::kStageDown))
	{
		color = 0xffffff;
	}
	else
	{
		color = 0x808080;
	}
	DrawTriangle(m_windowSize.w / 2, m_windowSize.h - 100,
		m_windowSize.w / 2 + 25, m_windowSize.h - 150,
		m_windowSize.w / 2 - 25, m_windowSize.h - 150,
		color, true);
}

void StageBase::SlideLeft(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:今からくそコード書くから後で直して
	// クリア情報の更新
	ChangeClearData(StageManager::kStageRight, nextStage->GetStageName());

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// 送る用の描画先を作成する
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	// 次のステージを下に動いていたらずらす
	DrawGraph(0, m_mgr.GetSlideVolumeY(StageManager::kStageDown), nextScreenHandle, true);
	// 現在の画像を上に動いていたらずらす
	DrawGraph(m_windowSize.w, m_mgr.GetSlideVolumeY(StageManager::kStageUp), nowScreenHandle, true);

	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	// 送る方は送ったほうで消すため考えない
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr.StartMove(StageManager::kStageLeft, sendScreenHandle);

	// 次のステージに変更する
	m_mgr.ChangeStage(nextStage);
}

void StageBase::SlideRight(std::shared_ptr<StageBase> nextStage)
{
	// FIXEME: クソコード書くから後で直して
	// クリア情報の更新
	ChangeClearData(StageManager::kStageLeft, nextStage->GetStageName());

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(0, m_mgr.GetSlideVolumeY(StageManager::kStageUp), nowScreenHandle, true);
	DrawGraph(m_windowSize.w, m_mgr.GetSlideVolumeY(StageManager::kStageDown), nextScreenHandle, true);

	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);
	
	// 画面を動かす処理を実行する
	m_mgr.StartMove(StageManager::kStageRight, sendScreenHandle);

	// 次のステージに変更する
	m_mgr.ChangeStage(nextStage);
}

void StageBase::SlideUp(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:今からくそコード書くから後で直して
	// クリア情報の更新
	ChangeClearData(StageManager::kStageDown, nextStage->GetStageName());

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);


	// 送る用の描画先を作成する
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr.GetSlideVolumeX(StageManager::kStageRight), 0, nextScreenHandle, true);
	DrawGraph(m_mgr.GetSlideVolumeX(StageManager::kStageLeft), m_windowSize.h, nowScreenHandle, true);
	
	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr.StartMove(StageManager::kStageUp, sendScreenHandle);

	// 次のステージに変更する
	m_mgr.ChangeStage(nextStage);
}

void StageBase::SlideDown(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:今からくそコード書くから後で直して
	// クリア情報の更新
	ChangeClearData(StageManager::kStageUp, nextStage->GetStageName());

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// 送る用の描画先を作成する
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr.GetSlideVolumeX(StageManager::kStageLeft), 0, nowScreenHandle, true);
	DrawGraph(m_mgr.GetSlideVolumeX(StageManager::kStageRight), m_windowSize.h, nextScreenHandle, true);

	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr.StartMove(StageManager::kStageDown, sendScreenHandle);

	// 次のステージに変更する
	m_mgr.ChangeStage(nextStage);
}

void StageBase::DrawWall()
{
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

	// 色は仮
	// 左
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0xffffff);
	// 右
	DrawLine(static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0xffffff);
	// 上
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		0xffffff);
	// 下
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0xffffff);
}

void StageBase::SlideStart(int& now, int& next, const std::shared_ptr<StageBase>& nextStage)
{
	// 現在の画面を保存するよう
	now = MakeScreen(m_windowSize.w, m_windowSize.h, true);
	// 描画先変更
	SetDrawScreen(now);
	// 現在の画面を描画
	Draw();
	// 次の画面を保持するよう
	next = MakeScreen(m_windowSize.w, m_windowSize.h, true);
	SetDrawScreen(next);
	nextStage->Draw();
}

void StageBase::ChangeClearData(int dir, const std::string& name) const
{
	m_mgr.SaveClear(name, dir);
}
