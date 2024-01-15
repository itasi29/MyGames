#include <DxLib.h>
#include <cassert>
#include "FontSystem.h"

FontSystem::FontSystem()
{
	// �ǂݍ��ރt�H���g�t�@�C���̃p�X
	m_fontPath = L"azuki.ttf"; 
	if (AddFontResourceEx(m_fontPath, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, L"�t�H���g�Ǎ����s", L"", MB_OK);
	}

	// �g�������ȃT�C�Y������Ă���
	m_handle[16] = CreateFontToHandle(m_fontPath, 16, -1);;
	m_handle[32] = CreateFontToHandle(m_fontPath, 32, -1);;
	m_handle[64] = CreateFontToHandle(m_fontPath, 64, -1);;
}

FontSystem::~FontSystem()
{
	// ������t�H���g�f�[�^��S�č폜
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
