#include "Input.h"
#include <DxLib.h>
#include <cassert>
#include <string>
#include <algorithm>

namespace
{
}

Input::Input() :
    m_isAnyPush(false),
    m_isLastAnyPush(false)
{
    m_commandTable["OK"] = { {InputType::keybd, { KEY_INPUT_RETURN } } ,
                             {InputType::pad,   { PAD_INPUT_1 } } };    // A (X-BOX)
    m_commandTable["shot"] = { {InputType::keybd,  { KEY_INPUT_SPACE } },
                                {InputType::pad,    { PAD_INPUT_1 } } };
    m_commandTable["up"] = { {InputType::keybd,  { KEY_INPUT_UP, KEY_INPUT_W } },
                             {InputType::pad,    { PAD_INPUT_UP } } };
    m_commandTable["down"] = { {InputType::keybd,  { KEY_INPUT_DOWN , KEY_INPUT_S }},
                             {InputType::pad,    { PAD_INPUT_DOWN } } };
    m_commandTable["left"] = { {InputType::keybd,  { KEY_INPUT_LEFT , KEY_INPUT_A }},
                             {InputType::pad,    { PAD_INPUT_LEFT  } } };
    m_commandTable["right"] = { {InputType::keybd,  { KEY_INPUT_RIGHT , KEY_INPUT_D }},
                             {InputType::pad,    { PAD_INPUT_RIGHT } } };
}

void Input::Update()
{
    m_lastInputDate = m_inputDate;  // ���O���͂��R�s�[���Ă���
    m_isLastAnyPush = m_isAnyPush;

    // �n�[�h�E�F�A���̓`�F�b�N
    // �L�[�{�[�h�p
    char keystate[256]; 
    GetHitKeyStateAll(keystate);   // ���݂̃L�[�{�[�h���͂��擾
    // �p�b�h���̎擾
    int padstate = GetJoypadInputState(DX_INPUT_PAD1);

    if (padstate || CheckHitKeyAll())
    {
        m_isAnyPush = true;
    }
    else
    {
        m_isAnyPush = false;
    }

    // ���X�V
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
                for (const auto& code : hardIO.second)
                {
                    if (keystate[code])
                    {
                        input = true;
                        break;
                    }
                }
                if (input) break;
            }
            else if (hardIO.first == InputType::pad)
            {
                for (const auto& code : hardIO.second)
                {
                    if (padstate & code)
                    {
                        input = true;
                        break;
                    }
                }
                if (input) break;
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

bool Input::IsAnyPress() const
{
    return m_isAnyPush;
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
