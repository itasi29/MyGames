#include "SceneDebug.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "Common/Input.h"

namespace
{
	/* ï∂éöån */
	const wchar_t* const kSceneName[SceneDebug::Kind::Max] =
	{
		{L"Title"},
		{L"Main"}
	};
	constexpr int kStrX = 16;
	constexpr int kStrY = 16;
	constexpr int kStrYAdd = 16;

	/* Boxån */
	constexpr int kBoxWidth = 64;
}

SceneDebug::SceneDebug() :
	m_kind(Kind::Title)
{
}

SceneDebug::~SceneDebug()
{
}

void SceneDebug::Init()
{
}

void SceneDebug::Final()
{
}

void SceneDebug::Update()
{
	auto& input = Input::GetInstance();

	int max = SceneDebug::Kind::Max;

	if (input.IsTrigger("down"))
	{
		m_kind = static_cast<SceneDebug::Kind>((m_kind + 1) % max);
	}
	if (input.IsTrigger("up"))
	{
		m_kind = static_cast<SceneDebug::Kind>((m_kind - 1 + max) % max);
	}

	if (input.IsTrigger("ok"))
	{
		m_scnMgr.Change();
	}
}

void SceneDebug::Draw() const
{
	// èÍèäï`âÊ
	int boxStartX = kStrX;
	int boxStartY = kStrY + kStrYAdd * m_kind;
	int boxEndX = kStrX + kBoxWidth;
	int boxEndY = boxStartY + kStrYAdd;
	DrawBox(boxStartX, boxStartY, boxEndX, boxEndY, 0xff0000, true);
	// ï∂éöï`âÊ
	int strY = kStrY;
	for (auto str : kSceneName)
	{
		DrawString(kStrX, strY, str, 0xffffff);
		strY += kStrYAdd;
	}
}

std::shared_ptr<SceneBase> SceneDebug::Change()
{
	std::shared_ptr<SceneBase> scene;

	if (m_kind == SceneDebug::Kind::Title)
	{
		scene = std::make_shared<SceneTitle>();
	}
	else if (m_kind == SceneDebug::Kind::Main)
	{
		scene = std::make_shared<SceneDebug>();
	}

	return scene;
}

