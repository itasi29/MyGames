#include "SceneTitle.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "SceneMain.h"
#include "Common/Input.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

void SceneTitle::Final()
{
}

void SceneTitle::Update()
{
	auto& input = Input::GetInstance();

	if (input.IsTrigger("ok"))
	{
		m_scnMgr.Change();
	}
}

void SceneTitle::Draw() const
{
	DrawString(16, 16, L"SceneTitle", 0xffffff);
}

std::shared_ptr<SceneBase> SceneTitle::Change()
{
	return std::shared_ptr<SceneMain>();
}
