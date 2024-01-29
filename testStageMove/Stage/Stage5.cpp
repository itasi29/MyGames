#include "Stage5.h"
#include "Stage2.h"
#include "Stage4.h"
#include "Stage6.h"
#include "Stage8.h"

Stage5::Stage5(StageManager& mgr) :
	StageBase(mgr)
{
	m_name = "5";
}

void Stage5::Update()
{
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		auto next = std::make_shared<Stage4>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		auto next = std::make_shared<Stage6>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{
		auto next = std::make_shared<Stage2>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		auto next = std::make_shared<Stage8>(m_mgr);
		m_mgr.ChangeScene(next);
	}
}
