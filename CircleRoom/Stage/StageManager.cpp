#include <DxLib.h>
#include <cassert>
#include "Application.h"

#include "StageManager.h"
#include "StageBase.h"

namespace
{
	constexpr int kStageMoveFrame = 30;
}

StageManager::StageManager() :
	m_frame(0),
	m_isStageMove(false),
	m_stageHandle(-1)
{
}

StageManager::~StageManager()
{
}

void StageManager::Update(Input& input)
{
	UpdateMove();
	m_stage->Update(input);
	m_stage->ChangeStage(input);
}

void StageManager::Draw()
{
	if (m_isStageMove)
	{
		DrawMove();
	}
	else
	{
		m_stage->Draw();
	}
}

void StageManager::ChangeStage(std::shared_ptr<StageBase> nextStage)
{
	m_stage = nextStage;
}

void StageManager::StartMove(MoveDir dir, int handle)
{
	m_isStageMove = true;
	m_frame = 0;

	switch (dir)
	{
	case StageManager::kDirLeft:
		m_pos.x = 0;
		m_pos.y = 0;

		m_vec.x = 1280 / kStageMoveFrame;
		m_vec.y = 0;
		break;
	case StageManager::kDirRight:
		m_pos.x = 1280;
		m_pos.y = 0;

		m_vec.x = -1280 / kStageMoveFrame;
		m_vec.y = 0;
		break;
	case StageManager::kDirUp:
		break;
	case StageManager::kDirDown:
		break;
	default:
		assert(false);
		break;
	}


	// 中身が入っていたらそれを消す
	if (m_stageHandle != 0)
	{
		DeleteGraph(m_stageHandle);
	}
	m_stageHandle = handle;
}

void StageManager::UpdateMove()
{
	if (!m_isStageMove) return;

	// フレームの更新
	m_frame++;
	// 場所の更新
	m_pos += m_vec;

	// 一定フレームたったら動かし完了とする
	if (m_frame >= kStageMoveFrame)
	{
		m_isStageMove = false;
	}
}

void StageManager::DrawMove()
{
	auto& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();
	
	DrawRectGraph(0, 0, m_pos.x, m_pos.y, size.w, size.h,
		m_stageHandle, true);
	//DrawGraph(-1280, 0, m_stageHandle, true);

#ifdef _DEBUG
	DrawFormatString(32, 32, 0xff0808, L"ステージ移動中 %d", m_frame);
	DrawFormatString(32, 48, 0xff0808, L"座標(%.2f, %.2f)", m_pos.x, m_pos.y);
#endif
}
