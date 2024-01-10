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

BottansFile::BottansFile(FileManager& mgr)
{
	m_handle = mgr.LoadGraphic(L"Data/Image/UI/xbox_buttons.png", true)->GetHandle();

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

void BottansFile::DrawBottan(std::wstring name, int drawX, int drawY) const
{
	auto it = m_bottans.find(name);
	if (it == m_bottans.end())
	{
		//assert(false);
		return;
	}

	// ‰æ‘œ‚Ì•`‰æ
	DrawRectGraph(drawX, drawY, 0, m_bottans.at(name) * kGraphSize, kGraphSize, kGraphSize, m_handle, true);
}

void BottansFile::DrawBottanToString(std::wstring name, int drawGraphX, int drawGraphY, int drawStrX, int drawStrY) const
{
	auto it = m_bottans.find(name);
	if (it == m_bottans.end())
	{
		assert(false);
		return;
	}

	// ‰æ‘œ‚Ì•`‰æ
	DrawRectGraph(drawGraphX, drawGraphY, 0, m_bottans.at(name) * kGraphSize, kGraphSize, kGraphSize, m_handle, true);

	// •¶Žš‚Ì•`‰æ
	DrawFormatString(drawStrX, drawStrY, 0xffffff, L"%s", name.c_str());
}
