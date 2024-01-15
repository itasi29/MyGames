#include <DxLib.h>
#include <cassert>
#include "FontSystem.h"

FontSystem::FontSystem()
{
	// 読み込むフォントファイルのパス
	m_fontPath = L"azuki.ttf"; 
	if (AddFontResourceEx(m_fontPath, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, L"フォント読込失敗", L"", MB_OK);
	}

	// 使いそうなサイズを作っておく
	m_handle[16] = CreateFontToHandle(m_fontPath, 16, -1);;
	m_handle[32] = CreateFontToHandle(m_fontPath, 32, -1);;
	m_handle[64] = CreateFontToHandle(m_fontPath, 64, -1);;
}

FontSystem::~FontSystem()
{
	// 作ったフォントデータを全て削除
	for (auto& handle : m_handle)
	{
		DeleteFontToHandle(handle.second);
	}
}

int FontSystem::GetHandle(int size) const
{
	auto it = m_handle.find(size);
	if (it == m_handle.end())
	{
		assert(false);
		return -1;
	}

	return it->second;
}
