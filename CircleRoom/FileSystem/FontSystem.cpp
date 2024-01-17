#include <DxLib.h>
#include <cassert>
#include "FontSystem.h"

namespace
{
	const wchar_t* const kFontName = L"���z�� �l�N�X�g UP B";
}

FontSystem::FontSystem()
{
	// �ǂݍ��ރt�H���g�t�@�C���̃p�X
	m_fontPath = L"Data/Font/Kaisotai-Next-UP-B.otf"; 
	if (AddFontResourceEx(m_fontPath, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, L"�t�H���g�Ǎ����s", L"", MB_OK);
	}

	// �g�������ȃT�C�Y������Ă���
	m_handle[24] = CreateFontToHandle(kFontName, 16, -1);;
	m_handle[32] = CreateFontToHandle(kFontName, 32, -1);;
	m_handle[64] = CreateFontToHandle(kFontName, 64, -1);;
	m_handle[72] = CreateFontToHandle(kFontName, 72, -1);;
}

FontSystem::~FontSystem()
{
	// ������t�H���g�f�[�^��S�č폜
	for (auto& handle : m_handle)
	{
		DeleteFontToHandle(handle.second);
	}

	if (RemoveFontResourceEx(m_fontPath, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, L"remove failure", L"", MB_OK);
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
