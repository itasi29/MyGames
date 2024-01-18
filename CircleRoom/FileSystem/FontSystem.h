#pragma once
#include <unordered_map>
#include <windows.h>

/// <summary>
/// �t�H���g�������N���X
/// </summary>
class FontSystem
{
public:
	FontSystem();
	~FontSystem();

	void Init();

	int GetHandle(int size) const;

private:
	LPCWSTR m_fontPath;

	// �t�H���g�̃n���h��
	// Key:�t�H���g�̃T�C�Y�@Value:�t�H���g�n���h��
	std::unordered_map<int, int> m_handle;
};

