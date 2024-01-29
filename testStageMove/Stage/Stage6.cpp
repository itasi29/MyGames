#include "Stage6.h"
#include "Stage3.h"
#include "Stage5.h"
#include "Stage9.h"

Stage6::Stage6(StageManager& mgr) :
	StageBase(mgr)
{
	m_name = "6";
}

void Stage6::Update()
{
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		auto next = std::make_shared<Stage5>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{
		auto next = std::make_shared<Stage3>(m_mgr);
		m_mgr.ChangeScene(next);

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		auto next = std::make_shared<Stage9>(m_mgr);
		m_mgr.ChangeScene(next);
	}
}
