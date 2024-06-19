#include "SceneMain.h"
#include <DxLib.h>
#include <cassert>
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
	m_frame = 0;
	m_handle = LoadGraph(L"Data/Image/nami.png");
	assert(m_handle != -1);
}

void SceneMain::Final()
{
	DeleteGraph(m_handle);
}

void SceneMain::Update()
{
	auto& input = Input::GetInstance();

	++m_frame;
	if (input.IsTrigger("ok"))
	{
		m_scnMgr.Change();
	}
}

void SceneMain::Draw() const
{
	DrawString(16, 16, L"SceneTitle", 0xffffff);
	DrawFormatString(16, 32, 0xffffff, L"Frame = %d", m_frame);
	DrawGraph(0, 0, m_handle, true);
}

std::shared_ptr<SceneBase> SceneMain::Change()
{
	return std::shared_ptr<SceneTitle>();
}
