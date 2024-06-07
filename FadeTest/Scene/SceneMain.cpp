#include "SceneMain.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "SceneTitle.h"
#include "Common/Input.h"

SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
}

void SceneMain::Final()
{
}

void SceneMain::Update()
{
	auto& input = Input::GetInstance();

	if (input.IsTrigger("ok"))
	{
		m_scnMgr.Change();
	}
}

void SceneMain::Draw() const
{
	DrawString(16, 16, L"SceneTitle", 0xffffff);
}

std::shared_ptr<SceneBase> SceneMain::Change()
{
	return std::shared_ptr<SceneTitle>();
}
