#include "Stage9.h"
#include "Stage8.h"
#include "Stage6.h"

Stage9::Stage9(StageManager& mgr) :
	StageBase(mgr)
{
	m_name = "9";
}

void Stage9::Update()
{
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		auto next = std::make_shared<Stage8>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{
		auto next = std::make_shared<Stage6>(m_mgr);
		m_mgr.ChangeScene(next);

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
	}
}
