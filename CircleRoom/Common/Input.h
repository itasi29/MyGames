#pragma once
#include <array>
#include <map>
#include <unordered_map>
#include <string>
#include "Vec2.h"

enum class InputType
{
	keybd,	// �L�[�{�[�h
	pad		// �p�b�h
};

using InputTable_t = std::unordered_map<std::string, std::map<InputType, int>>;

class KeyConfigScene;
class PadConfigScene;

class Input
{
	friend KeyConfigScene;
	friend PadConfigScene;
private:
	// �R�}���h���Ɠ��͂��y�A�ɂ����e�[�u��
	// �L�[�{�[�h�̎��� keybd&KEY_INPUT_�����ŁA
	// �p�b�h�̎���pad&PAD_INPUT_����
	// ���`�F�b�N����
	InputTable_t m_commandTable;
	std::vector<std::string> m_exclusiveKeyConfigCommands;

	// �R�}���h�̓��͂��o���Ă���
	std::map<std::string, bool> m_inputDate;		// ���݂̓���
	std::map<std::string, bool> m_lastInputDate;	// ���O�̓���

	// �X�e�B�b�N���
	Vec2 m_inputStickDate;
	// ��������̃{�^�������ꂽ���
	bool m_isAnyPush;
	bool m_isLastAnyPush;

	const InputTable_t GetCommandTable() const;
	std::vector<std::string> GetExclusiveCommandTable() const { return m_exclusiveKeyConfigCommands; }

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

	void Save(const std::string& path);
	void Load(const std::wstring& path);

	/// <summary>
	/// �X�e�B�b�N�̏���n��
	/// </summary>
	/// <returns>�X�e�B�b�N���</returns>
	Vec2 GetStickDate() const { return m_inputStickDate; }

	float GetStickTilt() const { return m_inputStickDate.Length(); }
};

