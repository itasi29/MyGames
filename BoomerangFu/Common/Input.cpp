#include "Input.h"
#include <DxLib.h>
#include <cassert>
#include <array>
#include "Game.h"

Input::Input()
{
	// ����
	m_commandTable["ok"] =		{ { Key, KEY_INPUT_RETURN },
								  { Pad, PAD_INPUT_1 }  };
	// �L�����Z��
	m_commandTable["cancel"] =  { { Key, KEY_INPUT_ESCAPE },
								  { Pad, PAD_INPUT_2 } };
	// �W�����v
	m_commandTable["jump"] =	{ { Key, KEY_INPUT_SPACE },
								  { Pad, PAD_INPUT_1 } };
	// �W�����v
	m_commandTable["attack"] =	{ { Key, KEY_INPUT_SPACE },
								  { Pad, PAD_INPUT_3 } };
	// ��
	m_commandTable["up"] =		{ { Key, KEY_INPUT_UP },
								  { Pad, PAD_INPUT_UP } };
	// ��
	m_commandTable["down"] =	{ { Key, KEY_INPUT_DOWN },
								  { Pad, PAD_INPUT_DOWN } };
	// �E
	m_commandTable["right"] =	{ { Key, KEY_INPUT_RIGHT },
								  { Pad, PAD_INPUT_RIGHT } };
	// ��
	m_commandTable["left"] =	{ { Key, KEY_INPUT_LEFT },
								  { Pad, PAD_INPUT_LEFT } };
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
	char keystate[256];
	GetHitKeyStateAll(keystate);

	// �S�ẴR�}���h�Ɣ���
	for (const auto& cmd : m_commandTable)
	{
		bool isCheck = false;
		// ���͂̎Q��
		auto& input = m_isInput[no][cmd.first];
		// ���͂̔���
		for (const auto& type : cmd.second)
		{
			// �L�[�`�F�b�N(0�Ԃ̂�)
			if (type.first == InputType::Key && no == 0)
			{
				isCheck = keystate[type.second];
			}
			// �p�b�h�`�F�b�N
			else if (type.first == InputType::Pad)
			{
				isCheck = type.second & padstate;
			}
			// true�Ȃ炱�̃R�}���h�I��
			if (isCheck)
			{
				input = true;
				break;
			}
		}
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
	// 5�Ԗ�
	else if (no == 4)
	{
		inputType = DX_INPUT_PAD5;
	}
	// 6�Ԗ�
	else if (no == 5)
	{
		inputType = DX_INPUT_PAD6;
	}
	// 7�Ԗ�
	else if (no == 6)
	{
		inputType = DX_INPUT_PAD7;
	}
	// 8�Ԗ�
	else if (no == 7)
	{
		inputType = DX_INPUT_PAD8;
	}
	// ���̑��̏ꍇ
	else
	{
		assert(false);
		inputType = DX_INPUT_PAD1;
	}

	return inputType;
}
