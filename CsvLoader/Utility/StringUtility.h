#pragma once
#include <string>
#include <vector>

class StringUtility
{
public:
	/// <summary>
	/// �}���`�o�C�g�����񂩂烏�C�h������ւ̕ϊ�
	/// </summary>
	/// <param name="str">�ϊ����̃}���`�o�C�g������</param>
	/// <returns>�ϊ���̃��C�h������</returns>
	static std::wstring  StringToWString(const std::string& str);

	/// <summary>
	/// �����֐�
	/// </summary>
	/// <param name="str">���̕�����</param>
	/// <param name="del">�������镶��</param>
	/// <returns>�������������Q</returns>
	static std::vector<std::string> Split(std::string& str, char del);
	/// <summary>
	/// �����֐�
	/// </summary>
	/// <param name="str">���̕�����</param>
	/// <param name="del">�������镶��</param>
	/// <returns>�������������Q</returns>
	static std::vector<std::wstring> Split(std::wstring& str, char del);
};

