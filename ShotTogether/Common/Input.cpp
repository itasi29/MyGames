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
    m_lastInputDate = m_inputDate;  // 直前入力をコピーしておく
    m_isLastAnyPush = m_isAnyPush;

    // ハードウェア入力チェック
    // キーボード用
    char keystate[256]; 
    GetHitKeyStateAll(keystate);   // 現在のキーボード入力を取得
    // パッド情報の取得
    int padstate = GetJoypadInputState(DX_INPUT_PAD1);

    if (padstate || CheckHitKeyAll())
    {
        m_isAnyPush = true;
    }
    else
    {
        m_isAnyPush = false;
    }

    // 情報更新
    for (const auto& cmd : m_commandTable)
    {
        // コマンドの名前から入力データを作る
        auto& input = m_inputDate[cmd.first];

        for (const auto& hardIO : cmd.second)
        {
            input = false;
            // キーボードのチェック
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
    // 押されていない時はフレームの初期化のみ行う
    if (IsNotPress(command))
    {
        frame = 0;
        return false;
    }
    // 押された瞬間はture
    if (IsTriggered(command))
    {
        frame = 0;
        return true;
    }

    frame++;
    // 間隔時間を超えたらtrueとする
    if (frame > frameInterval)
    {
        frame = 0;
        return true;
    }

    return false;
}
