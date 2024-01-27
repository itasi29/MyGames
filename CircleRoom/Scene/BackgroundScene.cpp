#include <DxLib.h>
#include "Application.h"
#include "BackgroundScene.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include <sstream>

namespace
{
	// îwåiâÊëúêî
	constexpr int kBgNum = 4;
}

BackgroundScene::BackgroundScene(std::shared_ptr<FileManager> mgr,  bool isMove, bool isDraw) :
	m_scroll(0),
	m_isMove(isMove),
	m_isDraw(isDraw)
{
	ChangeFunc();

	std::wostringstream oss;

	m_bg.resize(kBgNum);
	for (int i = 0; i < kBgNum; i++)
	{
		// wostringstreamÇÃèâä˙âª
		oss.str(L"");
		oss.clear();

		// ï∂éöóÒÇÃë„ì¸
		oss << "Bg/bg" << i << ".png";

		m_bg[i] = mgr->LoadGraphic(oss.str());
	}
}

BackgroundScene::~BackgroundScene()
{
}

void BackgroundScene::Update()
{
	if (!m_isDraw) return;

	(this->*m_updateFunc)();
}

void BackgroundScene::Draw() const
{
	if (!m_isDraw) return;

	(this->*m_drawFunc)();
}

void BackgroundScene::Init(bool isMove)
{
	m_scroll = 0;
	m_isMove = isMove;
	ChangeFunc();
}

void BackgroundScene::NormarUpdate()
{
}

void BackgroundScene::MoveUpdate()
{
	m_scroll -= 2;
}

void BackgroundScene::NormalDraw() const
{
//	DrawGraph(0, 0, m_bg->GetHandle(), true);
	for (const auto& bg : m_bg)
	{
		DrawGraph(0, 0, bg->GetHandle(), true);
	}
}

void BackgroundScene::MoveDraw() const
{
	const auto& size = Application::GetInstance().GetWindowSize();

	//int posX = m_frame % size.w;
	//DrawGraph(posX, 0, m_bg->GetHandle(), false);
	//posX += size.w;
	//DrawGraph(posX, 0, m_bg->GetHandle(), false);

	for (int i = 0; i < kBgNum; i++)
	{
		int posX = static_cast<int>(m_scroll * ((i + 1) / static_cast<float>(kBgNum))) % size.w;
		DrawGraph(posX, 0, m_bg[i]->GetHandle(), true);
		posX += size.w;
		DrawGraph(posX, 0, m_bg[i]->GetHandle(), true);
	}
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
