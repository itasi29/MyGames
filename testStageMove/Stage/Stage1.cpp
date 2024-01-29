#include <DxLib.h>
#include "Stage1.h"
#include "Stage2.h"
#include "Stage4.h"

Stage1::Stage1(StageManager& mgr) :
	StageBase(mgr)
{
	m_name = "1";
}

void Stage1::Update()
{
	if (CheckHitKey(KEY_INPUT_M))
	{
		m_mgr.DeathMoveScreen({1, 1});
	}

	if (CheckHitKey(KEY_INPUT_LEFT))
	{
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		auto next = std::make_shared<Stage2>(m_mgr);
		m_mgr.ChangeScene(next);
		return;
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{

	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		auto next = std::make_shared<Stage4>(m_mgr);
		m_mgr.ChangeScene(next);
	}
}
