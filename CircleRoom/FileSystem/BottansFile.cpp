#include <DxLib.h>
#include <cassert>
#include "StringUtility.h"

#include "BottansFile.h"

#include "FileManager.h"
#include "FileBase.h"

namespace
{
	constexpr int kGraphSize = 16;
}

BottansFile::BottansFile(std::shared_ptr<FileManager>& mgr)
{
	m_handle = mgr->LoadGraphic(L"UI/xbox_buttons.png", true)->GetHandle();

	m_bottans[L"‚`Bottan"] = 0;
	m_bottans[L"‚aBottan"] = 1;
	m_bottans[L"‚wBottan"] = 2;
	m_bottans[L"‚xBottan"] = 3;

//	m_bottans[L"c"] = mgr.LoadGraphic(L"c", true)->GetHandle();
	//m_bottans[L"z"] = mgr.LoadGraphic(L"z", true)->GetHandle();
	//m_bottans[L"l"] = mgr.LoadGraphic(L"l", true)->GetHandle();
	//m_bottans[L"r"] = mgr.LoadGraphic(L"r", true)->GetHandle();
	//m_bottans[L"start"] = mgr.LoadGraphic(L"start", true)->GetHandle();
	//m_bottans[L"m"] = mgr.LoadGraphic(L"m", true)->GetHandle();
}

BottansFile::~BottansFile()
{
}

void BottansFile::DrawBottan(std::wstring name, int drawX, int drawY, double rate) const
{
	auto it = m_bottans.find(name);
	if (it == m_bottans.end())
	{
		//assert(false);
		return;
	}

	// FIXME:–¼‘O‚Í•ÏX‚·‚é
	int aaa = static_cast<int>(kGraphSize * 0.5 * rate);
	drawX += aaa;
	drawY += aaa;

	// ‰æ‘œ‚Ì•`‰æ
	DrawRectRotaGraph(drawX, drawY, 0, m_bottans.at(name) * kGraphSize, 
		kGraphSize, kGraphSize, rate, 0.0, m_handle, true);
}

void BottansFile::DrawBottanAndString(std::wstring name, int drawGraphX, int drawGraphY, int drawStrX, int drawStrY, double rate) const
{
	DrawBottan(name, drawGraphX, drawGraphX, rate);

	// •¶š‚Ì•`‰æ
	DrawFormatString(drawStrX, drawStrY, 0xffffff, L"%s", name.c_str());
}
