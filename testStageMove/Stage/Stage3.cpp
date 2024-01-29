#include "Stage3.h"
#include "Stage2.h"
#include "Stage6.h"

Stage3::Stage3(StageManager& mgr) :
	StageBase(mgr)
{
	m_name = "3";
}

void Stage3::Update()
{
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		auto next = std::make_shared<Stage2>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		auto next = std::make_shared<Stage6>(m_mgr);
		m_mgr.ChangeScene(next);
	}
}
