#include <DxLib.h>
#include <cassert>
#include "StringUtility.h"

#include "FileManager.h"
#include "FileBase.h"

#include "KeyFile.h"

namespace
{
	constexpr int kGraphSize = 16;
}

KeyFile::KeyFile(std::shared_ptr<FileManager>& mgr)
{
	m_handle = mgr->LoadGraphic(L"Data/Image/UI/Keyboard.png", true)->GetHandle();

	m_key[L"�`�L�["] = { 0, 2 };
	m_key[L"�a�L�["] = { 1, 2 };
	m_key[L"�b�L�["] = { 2, 2 };
	m_key[L"�c�L�["] = { 3, 2 };
	m_key[L"�d�L�["] = { 4, 2 };
	m_key[L"�e�L�["] = { 5, 2 };
	m_key[L"�f�L�["] = { 6, 2 };
	m_key[L"�g�L�["] = { 7, 2 };
	m_key[L"�h�L�["] = { 0, 3 };
	m_key[L"�i�L�["] = { 1, 3 };
	m_key[L"�j�L�["] = { 2, 3 };
	m_key[L"�k�L�["] = { 3, 3 };
	m_key[L"�l�L�["] = { 4, 3 };
	m_key[L"�m�L�["] = { 5, 3 };
	m_key[L"�n�L�["] = { 6, 3 };
	m_key[L"�o�L�["] = { 7, 3 };
	m_key[L"�p�L�["] = { 0, 4 };
	m_key[L"�q�L�["] = { 1, 4 };
	m_key[L"�r�L�["] = { 2, 4 };
	m_key[L"�s�L�["] = { 3, 4 };
	m_key[L"�t�L�["] = { 4, 4 };
	m_key[L"�u�L�["] = { 5, 4 };
	m_key[L"�v�L�["] = { 6, 4 };
	m_key[L"�w�L�["] = { 7, 4 };
	m_key[L"�x�L�["] = { 0, 5 };
	m_key[L"�y�L�["] = { 1, 5 };
	m_key[L"BS�L�["] = { 14, 0, true };
	m_key[L"Tab�L�["] = { 8, 0, true };
	m_key[L"Enter�L�["] = { 14, 1, true };
	m_key[L"��Shift�L�["] = { 8, 1, true };
	m_key[L"�EShift�L�["] = { 8, 1, true };
	m_key[L"��Ctrl�L�["] = { 8, 2, true };
	m_key[L"�ECtrl�L�["] = { 8, 2, true };
	m_key[L"Esc�L�["] = { 10, 0, true };
	m_key[L"�X�y�[�X�L�["] = { 12, 2, true };
}

KeyFile::~KeyFile()
{
}

void KeyFile::DrawKey(std::wstring name, int drawX, int drawY, double rate) const
{
	auto it = m_key.find(name);
	if (it == m_key.end())
	{
		//assert(false);
		return;
	}

	// FIXME:���O�͕ύX����
	int aaa = (kGraphSize * 0.5 * rate);
	drawX += aaa;
	drawY += aaa;

	// �摜�̕`��
	int width = kGraphSize;
	if (it->second.isLink)
	{
		width += kGraphSize;
	}

	DrawRectRotaGraph(drawX, drawY, it->second.x * kGraphSize, it->second.y * kGraphSize,
		width, kGraphSize, rate, 0.0, m_handle, true);
}
