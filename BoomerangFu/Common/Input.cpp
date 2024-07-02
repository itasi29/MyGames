#include "Input.h"
#include <DxLib.h>
#include <cassert>
#include <array>
#include "Game.h"

Input::Input()
{
	// 決定
	m_commandTable["ok"] =		{ { Key, KEY_INPUT_RETURN },
								  { Pad, PAD_INPUT_1 }  };
	// キャンセル
	m_commandTable["cancel"] =  { { Key, KEY_INPUT_ESCAPE },
								  { Pad, PAD_INPUT_2 } };
	// ジャンプ
	m_commandTable["jump"] =	{ { Key, KEY_INPUT_SPACE },
								  { Pad, PAD_INPUT_1 } };
	// ジャンプ
	m_commandTable["attack"] =	{ { Key, KEY_INPUT_SPACE },
								  { Pad, PAD_INPUT_3 } };
	// 上
	m_commandTable["up"] =		{ { Key, KEY_INPUT_UP },
								  { Pad, PAD_INPUT_UP } };
	// 下
	m_commandTable["down"] =	{ { Key, KEY_INPUT_DOWN },
								  { Pad, PAD_INPUT_DOWN } };
	// 右
	m_commandTable["right"] =	{ { Key, KEY_INPUT_RIGHT },
								  { Pad, PAD_INPUT_RIGHT } };
	// 左
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
	// 事前の情報を保存
	m_isLastInput = m_isInput;

	// 入力情報の取得
	int padstate = GetJoypadInputState(GetInputType(no));
	char keystate[256];
	GetHitKeyStateAll(keystate);

	// 全てのコマンドと判定
	for (const auto& cmd : m_commandTable)
	{
		bool isCheck = false;
		// 入力の参照
		auto& input = m_isInput[no][cmd.first];
		// 入力の判定
		for (const auto& type : cmd.second)
		{
			// キーチェック(0番のみ)
			if (type.first == InputType::Key && no == 0)
			{
				isCheck = keystate[type.second];
			}
			// パッドチェック
			else if (type.first == InputType::Pad)
			{
				isCheck = type.second & padstate;
			}
			// trueならこのコマンド終了
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

	// 1番目
	if (no == 0)
	{
		inputType = DX_INPUT_PAD1;
	}
	// 2番目
	else if (no == 1)
	{
		inputType = DX_INPUT_PAD2;
	}
	// 3番目
	else if (no == 2)
	{
		inputType = DX_INPUT_PAD3;
	}
	// 4番目
	else if (no == 3)
	{
		inputType = DX_INPUT_PAD4;
	}
	// 5番目
	else if (no == 4)
	{
		inputType = DX_INPUT_PAD5;
	}
	// 6番目
	else if (no == 5)
	{
		inputType = DX_INPUT_PAD6;
	}
	// 7番目
	else if (no == 6)
	{
		inputType = DX_INPUT_PAD7;
	}
	// 8番目
	else if (no == 7)
	{
		inputType = DX_INPUT_PAD8;
	}
	// その他の場合
	else
	{
		assert(false);
		inputType = DX_INPUT_PAD1;
	}

	return inputType;
}
