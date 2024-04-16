#pragma once
#include <array>
#include <map>
#include <unordered_map>
#include <string>

enum class InputType
{
	keybd,	// �L�[�{�[�h
	pad		// �p�b�h
};

using InputTable_t = std::unordered_map<std::string, std::map<InputType, std::vector<int>>>;

class KeyConfigScene;
class PadConfigScene;

class Input
{
	friend KeyConfigScene;
	friend PadConfigScene;
private:
	// �R�}���h���Ɠ��͂��y�A�ɂ����e�[�u��
	InputTable_t m_commandTable;

	// �R�}���h�̓��͂��o���Ă���
	std::map<std::string, bool> m_inputDate;		// ���݂̓���
	std::map<std::string, bool> m_lastInputDate;	// ���O�̓���

	// ��������̃{�^�������ꂽ���
	bool m_isAnyPush;
	bool m_isLastAnyPush;

public:
	Input();

	/// <summary>
	/// ���͂̏����X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �w��̃R�}���h�������ꂽ�u�ԂȂ̂�
	/// </summary>
	/// <param name="command">�R�}���h������</param>
	/// <returns>true:�����ꂽ�u�� / false:������Ă��Ȃ�����������</returns>
	bool IsTriggered(const char* command) const;
	/// <summary>
	/// ��������̃{�^���������ꂽ��
	/// </summary>
	/// <returns>true:�����ꂽ�u�� / false:������Ă��Ȃ�����������</returns>
	bool IsAnyTriggerd();
	/// <summary>
	/// �w��̃R�}���h��������Ă��邩
	/// </summary>
	/// <param name="command">�R�}���h������</param>
	/// <returns>true:������Ă��� / false:������Ă��Ȃ�</returns>
	bool IsPress(const char* command) const;
	/// <summary>
	/// ��������̃{�^����������Ă��邩
	/// </summary>
	/// <returns>true:������Ă��� / false:������Ă��Ȃ�</returns>
	bool IsAnyPress() const;
	/// <summary>
	/// �w��̃R�}���h��������Ă��Ȃ���
	/// </summary>
	/// <param name="command">�R�}���h������</param>
	/// <returns>true:������Ă��Ȃ� / false:������Ă���</returns>
	bool IsNotPress(const char* command) const;
	/// <summary>
	/// �w��̃R�}���h�������ꂽ�u�ԂȂ̂�
	/// </summary>
	/// <param name="command">�R�}���h������</param>
	/// <returns>true:�����ꂽ�u�� / false:����ȊO</returns>
	bool IsReleased(const char* command) const;
	/// <summary>
	/// �����ꂽ�u�ԁ{������čs���Ď��Ԃ��o�߂���Ă���
	/// </summary>
	/// <param name="command">�R�}���h������</param>
	/// <param name="frame">�t���[���̎Q��</param>
	/// <param name="frameInterval">�t���[���̊Ԋu</param>
	/// <returns></returns>
	bool IsReepat(const char* command, int& frame, int frameInterval) const;
};

