#include "Input.h"
#include <DxLib.h>
#include <cassert>

Input::Input()
{
	m_commandTable["ok"] = PAD_INPUT_1;
	m_commandTable["jump"] = PAD_INPUT_1;
	m_commandTable["hover"] = PAD_INPUT_1;
	m_commandTable["up"] = PAD_INPUT_UP;
	m_commandTable["down"] = PAD_INPUT_DOWN;
	m_commandTable["right"] = PAD_INPUT_RIGHT;
	m_commandTable["left"] = PAD_INPUT_LEFT;
}

Input& Input::GetInstance()
{
	static Input input;
	return input;
}

void Input::Init()
{
}

void Input::Update(int no)
{
	// ���O�̏���ۑ�
	m_isLastInput = m_isInput;

	// ���͏��̎擾
	int padstate = GetJoypadInputState(GetInputType(no));

	// �S�ẴR�}���h�Ɣ���
	for (const auto& cmd : m_commandTable)
	{
		// ���͂̎Q��
		auto& input = m_isInput[no][cmd.first];

		// ���͂̔���
		input = cmd.second & padstate;
	}
}

bool Input::IsPress(int no, const char* const command) const
{
	return m_isInput.at(no).at(command);
}

bool Input::IsTriggerd(int no, const char* const command) const
{
	return m_isInput.at(no).at(command) && !m_isLastInput.at(no).at(command);
}

bool Input::IsReleased(int no, const char* const command) const
{
	return !m_isInput.at(no).at(command) && m_isLastInput.at(no).at(command);
}

int Input::GetInputType(int no)
{
	int inputType;

	// 1�Ԗ�
	if (no == 0)
	{
		inputType = DX_INPUT_PAD1;
	}
	// 2�Ԗ�
	else if (no == 1)
	{
		inputType = DX_INPUT_PAD2;
	}
	// 3�Ԗ�
	else if (no == 2)
	{
		inputType = DX_INPUT_PAD3;
	}
	// 4�Ԗ�
	else if (no == 3)
	{
		inputType = DX_INPUT_PAD4;
	}
	// ���̑��̏ꍇ
	else
	{
		assert(false);
		inputType = DX_INPUT_PAD1;
	}

	return inputType;
}
