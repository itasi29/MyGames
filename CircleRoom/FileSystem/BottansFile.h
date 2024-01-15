#pragma once
#include <string>
#include <unordered_map>
#include <memory>

/// <summary>
/// �ėpBottan
/// </summary>

class FileManager;

class BottansFile
{
public:
	BottansFile(std::shared_ptr<FileManager>& mgr);
	~BottansFile();

	/// <summary>
	/// �{�^���̉摜�̂ݕ`��
	/// </summary>
	/// <param name="name">Bottan��</param>
	/// <param name="drawX">���[</param>
	/// <param name="drawY">��[</param>
	/// <param name="rate">�{��</param>
	void DrawBottan(std::wstring name, int drawX, int drawY, double rate = 1.0) const;
	/// <summary>
	/// �{�^���̉摜�{�����`��
	/// </summary>
	/// <param name="name">Bottan��</param>
	/// <param name="drawGraphX">�摜���[</param>
	/// <param name="drawGraphY">�摜��[</param>
	/// <param name="drawStrX">�������[</param>
	/// <param name="drawStrY">������[</param>
	/// <param name="rate">�{��</param>
	void DrawBottanAndString(std::wstring name, int drawGraphX, int drawGraphY, int drawStrX, int drawStrY, double rate = 1.0) const;

private:
	int m_handle;

	// �{�^�����@�؂蔲���͈�
	std::unordered_map<std::wstring, int> m_bottans;	
};

