#include "Stage7.h"
#include "Stage4.h"
#include "Stage8.h"

Stage7::Stage7(StageManager& mgr) :
	StageBase(mgr)
{
	m_name = "7";
}

void Stage7::Update()
{
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		auto next = std::make_shared<Stage8>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{
		auto next = std::make_shared<Stage4>(m_mgr);
		m_mgr.ChangeScene(next);

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
	}
}
