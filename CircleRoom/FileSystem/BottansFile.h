#pragma once
#include <string>
#include <unordered_map>

/// <summary>
/// �ėpBottan
/// </summary>

class FileManager;

class BottansFile
{
public:
	BottansFile(FileManager& mgr);
	~BottansFile();

	/// <summary>
	/// �{�^���̉摜�̂ݕ`��
	/// </summary>
	/// <param name="name">Bottan��</param>
	/// <param name="drawX">���[</param>
	/// <param name="drawY">��[</param>
	void DrawBottan(std::wstring name, int drawX, int drawY) const;
	/// <summary>
	/// �{�^���̉摜�{�����`��
	/// </summary>
	/// <param name="name">Bottan��</param>
	/// <param name="drawGraphX">�摜���[</param>
	/// <param name="drawGraphY">�摜��[</param>
	/// <param name="drawStrX">�������[</param>
	/// <param name="drawStrY">������[</param>
	void DrawBottanToString(std::wstring name, int drawGraphX, int drawGraphY, int drawStrX, int drawStrY) const;

private:
	int m_handle;

	// �{�^�����@�؂蔲���͈�
	std::unordered_map<std::wstring, int> m_bottans;
};

