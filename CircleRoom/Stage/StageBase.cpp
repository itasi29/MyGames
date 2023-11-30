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

void StageBase::DrawStageConditions()
{
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
	DrawFormatString(128, 32, 0xffffff, L"00:00.000");
	// ステージ条件の描画
	/*一時的に条件とだけ描画*/
	//DrawStageConditions();
	DrawString(128, 48, L"条件", 0xffffff);
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
	DrawFormatString(128, 32, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);
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
