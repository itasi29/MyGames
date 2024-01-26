#include <DxLib.h>
#include "Application.h"
#include "BackgroundScene.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"

BackgroundScene::BackgroundScene(std::shared_ptr<FileManager> mgr,  bool isMove) :
	m_frame(0),
	m_isMove(isMove)
{
	ChangeFunc();

	m_bg = mgr->LoadGraphic(L"Bg/bg.png");
}

BackgroundScene::~BackgroundScene()
{
}

void BackgroundScene::Update()
{
	(this->*m_updateFunc)();
}

void BackgroundScene::Draw() const
{
	(this->*m_drawFunc)();
}

void BackgroundScene::Init(bool isMove)
{
	m_frame = 0;
	m_isMove = isMove;
	ChangeFunc();
}

void BackgroundScene::NormarUpdate()
{
}

void BackgroundScene::MoveUpdate()
{
	m_frame--;
}

void BackgroundScene::NormalDraw() const
{
	DrawGraph(0, 0, m_bg->GetHandle(), true);
}

void BackgroundScene::MoveDraw() const
{
	const auto& size = Application::GetInstance().GetWindowSize();

	int posX = m_frame % size.w;
	DrawGraph(posX, 0, m_bg->GetHandle(), false);
	posX += size.w;
	DrawGraph(posX, 0, m_bg->GetHandle(), false);
}

void BackgroundScene::ChangeFunc()
{
	if (m_isMove)
	{
		m_updateFunc = &BackgroundScene::MoveUpdate;
		m_drawFunc = &BackgroundScene::MoveDraw;
	}
	else
	{
		m_updateFunc = &BackgroundScene::NormarUpdate;
		m_drawFunc = &BackgroundScene::NormalDraw;
	}
}
