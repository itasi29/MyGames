#include "Stage8.h"
#include "Stage7.h"
#include "Stage9.h"
#include "Stage5.h"

Stage8::Stage8(StageManager& mgr) :
	StageBase(mgr)
{
	m_name = "8";
}

void Stage8::Update()
{
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		auto next = std::make_shared<Stage7>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		auto next = std::make_shared<Stage9>(m_mgr);
		m_mgr.ChangeScene(next);
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{
		auto next = std::make_shared<Stage5>(m_mgr);
		m_mgr.ChangeScene(next);

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
	}
}
