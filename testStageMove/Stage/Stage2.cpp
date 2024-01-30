#include "Stage2.h"
#include "Stage1.h"
#include "Stage3.h"
#include "Stage5.h"

Stage2::Stage2(StageManager& mgr) :
	StageBase(mgr)
{
	m_name = "2";
}

void Stage2::Update()
{
	if (CheckHitKey(KEY_INPUT_M))
	{
		m_mgr.DeathMoveScreen({ -1, 1 });
	}

	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		auto next = std::make_shared<Stage1>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		auto next = std::make_shared<Stage3>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		auto next = std::make_shared<Stage5>(m_mgr);
		m_mgr.ChangeScene(next);
	}
}
