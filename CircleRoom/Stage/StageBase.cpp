#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageBase.h"
#include "StageManager.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"

StageBase::StageBase(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	m_mgr(mgr),
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_frame(0),
	m_bestTime(0),
	m_waitFrame(0)
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

void StageBase::UpdateSelect(Input& input)
{
	// エネミーだけ動く処理繰り返す
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();
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
	// プレイヤーの情報を抜き取る
	bool playerIsDash = m_player->IsDash();
	bool playerIsExsit = m_player->IsExsit();
	const Rect playerRect = m_player->GetRect();

	CreateEnemy();
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();

		// プレイヤーとの当たり判定処理
		// ダッシュしていたら処理はしない
		if (!playerIsDash && playerRect.IsCollsion(enemy->GetRect()))
		{
			// プレイヤーの死亡処理
			m_player->Death();

			// メンバ関数ポインタを選択の方に戻す
			m_updateFunc = &StageBase::UpdateSelect;
			m_drawFunc = &StageBase::DrawSelect;

			// フレームの初期化
			m_waitFrame = 0;
			// クリアしているかの確認
			CheckStageConditions();

			// ベストタイムの更新
			if (m_bestTime < m_frame)
			{
				m_bestTime = m_frame;
			}
		}
	}

	// 経過時間の更新
	m_frame++;
	m_player->Update(input);
}

void StageBase::DrawSelect()
{
	// todo:描画は今のところ同じだが、今後は変えるようにする

	DrawWall();

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	// ステージ名の描画
	DrawFormatString(128, 16, 0xffffff, L"%s", m_stageName.c_str());
	// 時間の描画
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(128, 32, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);
	// ステージ条件の描画
	DrawStageConditions();
}

void StageBase::DrawPlaying()
{
	DrawWall();

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
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
	DrawStageConditions(true);
}

void StageBase::SlideLeft(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:今からくそコード書くから後で直して

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// 送る用の描画先を作成する
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	// 次のステージを下に動いていたらずらす
	DrawGraph(0, m_mgr->GetSlideVolumeY(StageManager::kDirDown), nextScreenHandle, true);
	// 現在の画像を上に動いていたらずらす
	DrawGraph(m_windowSize.w, m_mgr->GetSlideVolumeY(StageManager::kDirUp), nowScreenHandle, true);

	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	// 送る方は送ったほうで消すため考えない
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr->StartMove(StageManager::kDirLeft, sendScreenHandle);

	// 次のステージに変更する
	m_mgr->ChangeStage(nextStage);
}

void StageBase::SlideRight(std::shared_ptr<StageBase> nextStage)
{
	// FIXEME: クソコード書くから後で直して

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(0, m_mgr->GetSlideVolumeY(StageManager::kDirUp), nowScreenHandle, true);
	DrawGraph(m_windowSize.w, m_mgr->GetSlideVolumeY(StageManager::kDirDown), nextScreenHandle, true);

	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr->StartMove(StageManager::kDirRight, sendScreenHandle);

	// 次のステージに変更する
	m_mgr->ChangeStage(nextStage);
}

void StageBase::SlideUp(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:今からくそコード書くから後で直して

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);


	// 送る用の描画先を作成する
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr->GetSlideVolumeX(StageManager::kDirRight), 0, nextScreenHandle, true);
	DrawGraph(m_mgr->GetSlideVolumeX(StageManager::kDirLeft), m_windowSize.h, nowScreenHandle, true);
	
	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr->StartMove(StageManager::kDirUp, sendScreenHandle);

	// 次のステージに変更する
	m_mgr->ChangeStage(nextStage);
}

void StageBase::SlideDown(std::shared_ptr<StageBase> nextStage)
{
	// FIXME:今からくそコード書くから後で直して

	// 画面を保存するよう
	int nowScreenHandle, nextScreenHandle;
	SlideStart(nowScreenHandle, nextScreenHandle, nextStage);

	// 送る用の描画先を作成する
	int sendScreenHandle;
	sendScreenHandle = MakeScreen(m_windowSize.w * 2, m_windowSize.h * 2, true);
	SetDrawScreen(sendScreenHandle);
	DrawGraph(m_mgr->GetSlideVolumeX(StageManager::kDirLeft), 0, nowScreenHandle, true);
	DrawGraph(m_mgr->GetSlideVolumeX(StageManager::kDirRight), m_windowSize.h, nextScreenHandle, true);

	// 描画先を元の場所に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 現在、次の画面を保存したハンドルは宙づりになるのでここで消す
	DeleteGraph(nowScreenHandle);
	DeleteGraph(nextScreenHandle);

	// 画面を動かす処理を実行する
	m_mgr->StartMove(StageManager::kDirDown, sendScreenHandle);

	// 次のステージに変更する
	m_mgr->ChangeStage(nextStage);
}

void StageBase::DrawWall()
{
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

	// 色は仮
	// 左
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);
	// 右
	DrawLine(static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);
	// 上
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		0x00ff00);
	// 下
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);
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