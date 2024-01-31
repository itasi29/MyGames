#include "Input.h"
#include <DxLib.h>
#include <cassert>
#include <string>
#include <algorithm>

using namespace std;

namespace
{
    struct KeyConfHeader
    {
        char id[4] = "kyc"; // �Ō��'\0'�����Ă�̂�4�o�C�g
        float version = 1.0f;
        size_t dataCount = 0;
        // �󔒂�4�o�C�g(�p�f�B���O)
    };
}

const InputTable_t Input::GetCommandTable() const
{
    InputTable_t ret = m_commandTable;
    for (const auto& ex : m_exclusiveKeyConfigCommands)
    {
        ret.erase(ex);
    }

    return ret;
}

Input::Input() :
    m_isAnyPush(false),
    m_isLastAnyPush(false),
    m_lastType(InputType::pad)
{
    // ���j���[�֘A
    // ���������
    m_commandTable["OK"] = { {InputType::keybd, KEY_INPUT_RETURN} ,
                             {InputType::pad,   PAD_INPUT_A} };    
    m_commandTable["cancel"] = { {InputType::keybd, KEY_INPUT_ESCAPE} ,
                             {InputType::pad,   PAD_INPUT_B} };     // 
    m_commandTable["pause"] = { {InputType::keybd,  KEY_INPUT_P},
                                {InputType::pad,    PAD_INPUT_R} }; // �X�^�[�g�{�^��
    m_commandTable["keyconf"] = { {InputType::keybd,  KEY_INPUT_K},
                                {InputType::pad,    PAD_INPUT_L} }; // �L�[�R���t�B�O
    // �����Ȃ�����
    m_commandTable["optionLeft"] = { {InputType::keybd, KEY_INPUT_Q},
                                     {InputType::pad,   PAD_INPUT_5} };
    m_commandTable["optionRight"] = { {InputType::keybd, KEY_INPUT_E},
                                     {InputType::pad,   PAD_INPUT_6} };

    // �Q�[�����֘A
    // ���������
    m_commandTable["dash"] = { {InputType::keybd,  KEY_INPUT_SPACE},
                                {InputType::pad,    PAD_INPUT_A} }; // �L�[�R���t�B�O

    // �����Ȃ�����
    m_commandTable["up"] = { {InputType::keybd,  KEY_INPUT_UP},
                             {InputType::pad,    PAD_INPUT_UP} };
    m_commandTable["down"] = { {InputType::keybd,  KEY_INPUT_DOWN},
                             {InputType::pad,    PAD_INPUT_DOWN} };
    m_commandTable["left"] = { {InputType::keybd,  KEY_INPUT_LEFT},
                             {InputType::pad,    PAD_INPUT_LEFT} };
    m_commandTable["right"] = { {InputType::keybd,  KEY_INPUT_RIGHT},
                             {InputType::pad,    PAD_INPUT_RIGHT} };

    m_exclusiveKeyConfigCommands = {"optionLeft", "optionRight", "up", "down", "left", "right"};

}

void Input::Update()
{
    m_lastInputDate = m_inputDate;  // ���O���͂��R�s�[���Ă���
    m_isLastAnyPush = m_isAnyPush;
    m_isAnyPush = false;

    /* �n�[�h�E�F�A���̓`�F�b�N */
    // �L�[�{�[�h�p
    char keystate[256]; 
    GetHitKeyStateAll(keystate);   // ���݂̃L�[�{�[�h���͂��擾
    // �p�b�h���̎擾
    int padstate = GetJoypadInputState(DX_INPUT_PAD1);  
    // �X�e�B�b�N���̎擾
    int stickX, stickY;
    GetJoypadAnalogInput(&stickX, &stickY, DX_INPUT_PAD1);
    m_inputStickDate.x = static_cast<float>(stickX);
    m_inputStickDate.y = static_cast<float>(stickY);

    if (padstate || CheckHitKeyAll())
    {
        m_isAnyPush = true;
    }

    /*���X�V*/
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
                    m_lastType = hardIO.first;
                    break;
                }
            }
            else if (hardIO.first == InputType::pad)
            {
                if (padstate & hardIO.second)
                {
                    input = true;
                    m_lastType = hardIO.first;
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

bool Input::IsAnyTriggerd()
{
    return m_isAnyPush && !m_isLastAnyPush;
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

bool Input::IsNotPress(const char* command) const
{
    auto it = m_inputDate.find(command);
    if (it == m_inputDate.end())
    {
        assert(false);
        return false;
    }

    return !m_inputDate.at(command);
}

bool Input::IsReleased(const char* command) const
{
    auto it = m_inputDate.find(command);
    if (it == m_inputDate.end())
    {
        assert(false);
        return false;
    }

    return !m_inputDate.at(command) && m_lastInputDate.at(command);
}

bool Input::IsReepat(const char* command, int& frame, int frameInterval) const
{
    // ������Ă��Ȃ����̓t���[���̏������̂ݍs��
    if (IsNotPress(command))
    {
        frame = 0;
        return false;
    }
    // �����ꂽ�u�Ԃ�ture
    if (IsTriggered(command))
    {
        frame = 0;
        return true;
    }

    frame++;
    // �Ԋu���Ԃ𒴂�����true�Ƃ���
    if (frame > frameInterval)
    {
        frame = 0;
        return true;
    }

    return false;
}
