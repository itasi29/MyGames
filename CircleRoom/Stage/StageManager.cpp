#include <DxLib.h>
#include "Application.h"

#include "StageManager.h"
#include "StageBase.h"

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

void StageManager::StartMove(const Vec2& vec, int handle)
{
	m_isStageMove = true;
	m_frame = 0;

	m_pos.x += m_vec.x * 1280;
	m_pos.y += m_vec.y * 720;

	m_vec = m_pos / 60;

	// ’†g‚ª“ü‚Á‚Ä‚¢‚½‚ç‚»‚ê‚ðÁ‚·
	if (m_stageHandle != 0)
	{
		DeleteGraph(m_stageHandle);
	}
	m_stageHandle = handle;
}

void StageManager::UpdateMove()
{
	if (!m_isStageMove) return;

	m_frame++;

	if (m_frame >= 60)
	{
		m_isStageMove = false;
	}
}

void StageManager::DrawMove()
{
	auto& app = Application::GetInstance();
	const auto& size = app.GetWindowSize();
	
	// todo:‰æ–ÊˆÚ‚é‚æ‚¤‚É‚·‚é
	DrawGraph(size.w - m_pos.x, size.h - m_pos.y,
		m_stageHandle, true);
}
