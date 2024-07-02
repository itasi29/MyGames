#pragma once
#include <unordered_map>
#include <string>

enum InputType
{
	Key,
	Pad
};

using InputTable_t = std::unordered_map<std::string, std::unordered_map<InputType, int>>;
using InputData_t = std::unordered_map<int, std::unordered_map<std::string, bool>>;

class Input
{
private :
	Input();

	Input(const Input& input) = delete;
	void operator=(const Input& input) = delete;

public:
	static Input& GetInstance();

	/// <summary>
	/// ����������
	/// </summary>
	void Init();
	/// <summary>
	/// ���͏��̍X�V
	/// </summary>
	/// <param name="no">�^�C�v�ԍ�</param>
	void Update(int no);

	/// <summary>
	/// ������Ă��邩
	/// </summary>
	/// <param name="no">�^�C�v�ԍ�</param>
	/// <param name="command">�R�}���h</param>
	/// <returns>true : �����Ă��� / false : �����Ă��Ȃ�</returns>
	bool IsPress(int no, const char* const command) const;
	/// <summary>
	/// �������u��
	/// </summary>
	/// <param name="no">�^�C�v�ԍ�</param>
	/// <returns>true : �������u�� / false : �����Ă��Ȃ�or���������Ă���</returns>
	bool IsTriggerd(int no, const char* const command) const;
	/// <summary>
	/// �������u��
	/// </summary>
	/// <param name="no">�^�C�v�ԍ�</param>
	/// <returns>true : ������ / false : �����Ă��Ȃ�or���������Ă���</returns>
	bool IsReleased(int no, const char* const command) const;

private:
	int GetInputType(int no);

private:
	// �R�}���h�Ɠ��͂̃y�A
	InputTable_t m_commandTable;

	// ���͂̋L��
	InputData_t m_isInput;		// ���݂̓���
	InputData_t m_isLastInput;	// ���O�̓���
};

