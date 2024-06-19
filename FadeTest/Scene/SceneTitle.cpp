#include "SceneTitle.h"
#include <DxLib.h>
#include <cassert>
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
	m_frame = 0;
	m_handle = LoadGraph(L"Data/Image/robin.png");
	assert(m_handle != -1);
}

void SceneTitle::Final()
{
	DeleteGraph(m_handle);
}

void SceneTitle::Update()
{
	auto& input = Input::GetInstance();

	++m_frame;
	if (input.IsTrigger("ok"))
	{
		m_scnMgr.Change();
	}
}

void SceneTitle::Draw() const
{
	DrawString(16, 16, L"SceneTitle", 0xffffff);
	DrawFormatString(16, 32, 0xffffff, L"Frame = %d", m_frame);
	DrawGraph(0, 0, m_handle, true);
}

std::shared_ptr<SceneBase> SceneTitle::Change()
{
	return std::shared_ptr<SceneMain>();
}
