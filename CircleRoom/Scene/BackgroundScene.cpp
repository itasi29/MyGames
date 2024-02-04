#include <DxLib.h>
#include "Application.h"
#include "BackgroundScene.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include <sstream>

namespace
{
	// スクロールスピード
	constexpr float kSpeed = 2.0f;

	// 背景画像数
	constexpr int kBgNum = 4;
}

BackgroundScene::BackgroundScene(std::shared_ptr<FileManager> mgr,  bool isMove, bool isDraw) :
	m_scroll(0),
	m_scrollSpeed(kSpeed),
	m_isMove(isMove),
	m_isDraw(isDraw)
{
	ChangeFunc();

#if false
	m_bg = mgr->LoadGraphic(L"Bg/bg.png");
#else
	m_bg.resize(kBgNum);
	for (int i = 0; i < kBgNum; i++)
	{
		std::wostringstream woss;
		woss << L"Bg/bg" << i << L".png";
		m_bg[i] = mgr->LoadGraphic(woss.str());
	}
#endif
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

void BackgroundScene::Init(float speed)
{
	if (speed < 0)
	{
		m_scrollSpeed = kSpeed;
	}
	else
	{
		m_scrollSpeed = speed;
	}

	m_scroll = 0;
	ChangeFunc();
}

void BackgroundScene::NormarUpdate()
{
}

void BackgroundScene::MoveUpdate()
{
	m_scroll -= m_scrollSpeed;
}

void BackgroundScene::NormalDraw() const
{
#if false
	DrawGraph(0, 0, m_bg->GetHandle(), true);
#else
	for (const auto& bg : m_bg)
	{
		DrawGraph(0, 0, bg->GetHandle(), true);
	}
#endif
}

void BackgroundScene::MoveDraw() const
{
#if false
	size size;
	GetGraphSize(m_bg->GetHandle(), &size.w, &size.h);

	int posX = static_cast<int>(m_scroll) % size.w;
	DrawGraph(posX, 0, m_bg->GetHandle(), false);
	posX += size.w;
	DrawGraph(posX, 0, m_bg->GetHandle(), false);
#else
	const auto& size = Application::GetInstance().GetWindowSize();

	for (int i = 0; i < kBgNum; i++)
	{
		int posX = static_cast<int>(m_scroll * ((i + 1) / static_cast<float>(kBgNum))) % size.w;
		DrawGraph(posX, 0, m_bg[i]->GetHandle(), true);
		posX += size.w;
		DrawGraph(posX, 0, m_bg[i]->GetHandle(), true);
	}
#endif
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
