#include "Input.h"
#include <DxLib.h>
#include <cassert>
#include <string>
#include <algorithm>

using namespace std;

const InputTable_t Input::GetCommandTable() const
{
    InputTable_t ret = m_commandTable;
    for (const auto& ex : m_exclusiveKeyConfigCommands)
    {
        ret.erase(ex);
    }

    return ret;
}

Input::Input()
{
    m_commandTable["OK"] = { {InputType::keybd, KEY_INPUT_RETURN} ,
                             {InputType::pad,   PAD_INPUT_A} };    
    m_commandTable["cancel"] = { {InputType::keybd, KEY_INPUT_ESCAPE} ,
                             {InputType::pad,   PAD_INPUT_B} };     // 
    m_commandTable["pause"] = { {InputType::keybd,  KEY_INPUT_P},
                                {InputType::pad,    PAD_INPUT_R} }; // �X�^�[�g�{�^��
    m_commandTable["keyconf"] = { {InputType::keybd,  KEY_INPUT_K},
                                {InputType::pad,    PAD_INPUT_L} }; // �L�[�R���t�B�O

    m_commandTable["up"] = { {InputType::keybd,  KEY_INPUT_UP},
                             {InputType::pad,    PAD_INPUT_UP} };
    m_commandTable["down"] = { {InputType::keybd,  KEY_INPUT_DOWN},
                             {InputType::pad,    PAD_INPUT_DOWN} };
    m_commandTable["left"] = { {InputType::keybd,  KEY_INPUT_LEFT},
                             {InputType::pad,    PAD_INPUT_LEFT} };
    m_commandTable["right"] = { {InputType::keybd,  KEY_INPUT_RIGHT},
                             {InputType::pad,    PAD_INPUT_RIGHT} };

    m_exclusiveKeyConfigCommands = {"up", "down", "left", "right"};
}

void Input::Update()
{
    m_lastInputDate = m_inputDate;  // ���O���͂��R�s�[���Ă���

    // �n�[�h�E�F�A���̓`�F�b�N
    char keystate[256]; // �L�[�{�[�h�p
    GetHitKeyStateAll(keystate);   // ���݂̃L�[�{�[�h���͂��擾
    int padstate = GetJoypadInputState(DX_INPUT_PAD1);  // �p�b�h���̎擾

    // �o�^���ꂽ���ƃn�[�h�̏����Ƃ炵���킹�Ȃ���A
    // m_inputData�̓��e���X�V���Ă���
    for (const auto& cmd : m_commandTable)
    {
        // �R�}���h�̖��O������̓f�[�^�����
        auto& input = m_inputDate[cmd.first];
        for (const auto& hardIO : cmd.second)
        {
            input = false;
            // �L�[�{�[�h�̃`�F�b�N
            if (hardIO.first == InputType::keybd)
            {
                if (keystate[hardIO.second])
                {
                    input = true;
                    break;
                }
            }
            else if (hardIO.first == InputType::pad)
            {
                if (padstate & hardIO.second)
                {
                    input = true;
                    break;
                }
            }
        }
    }
}

bool Input::IsTriggered(const char* command) const
{
    auto it = m_inputDate.find(command);
    if (it == m_inputDate.end())
    {
        assert(false);
        return false;
    }

    return m_inputDate.at(command) && !m_lastInputDate.at(command);

}

bool Input::IsPress(const char* command) const
{
    auto it = m_inputDate.find(command);
    if (it == m_inputDate.end())
    {
        assert(false);
        return false;
    }

    return m_inputDate.at(command);
}
