#include "Stage4.h"
#include "Stage1.h"
#include "Stage5.h"
#include "Stage7.h"

Stage4::Stage4(StageManager& mgr) :
	StageBase(mgr)
{
	m_name = "4";
}

void Stage4::Update()
{
	if (CheckHitKey(KEY_INPUT_M))
	{
		m_mgr.DeathMoveScreen({ 1, -1 });
	}

	if (CheckHitKey(KEY_INPUT_LEFT))
	{
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		auto next = std::make_shared<Stage5>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{
		auto next = std::make_shared<Stage1>(m_mgr);
		m_mgr.ChangeScene(next);

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		auto next = std::make_shared<Stage7>(m_mgr);
		m_mgr.ChangeScene(next);
	}
}
