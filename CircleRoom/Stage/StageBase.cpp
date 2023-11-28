#include <DxLib.h>
#include "Application.h"

#include "StageBase.h"
#include "StageManager.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"

StageBase::StageBase(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	m_mgr(mgr),
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_frame(0)
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

	// todo:AボタンかBボタンを押されたらプレイ中に移行するようにする
}

void StageBase::UpdatePlaying(Input& input)
{
	// プレイヤーの情報を抜き取る
	bool playerIsDash = m_player->IsDash();
	bool playerIsExsit = m_player->IsExsit();
	const Rect playerRect = m_player->GetRect();

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
		}
	}

	// 経過時間の更新
	m_frame++;
	m_player->Update(input);
}

void StageBase::DrawSelect()
{
	// todo:描画は今のところ同じだが、今後は変えるようにする

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	// 時間の描画
	int minSec = (m_frame * 100 / 60) % 100;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(m_windowSize.w / 2 - 32, 32, 0xffffff, L"%02d:%02d.%02d", min, sec, minSec);
}

void StageBase::DrawPlaying()
{
	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	// 時間の描画
	int minSec = (m_frame * 100 / 60) % 100;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(m_windowSize.w / 2 - 32, 32, 0xffffff, L"%02d:%02d.%02d", min, sec, minSec);
}
