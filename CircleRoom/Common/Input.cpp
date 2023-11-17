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
    // メニュー関連
    m_commandTable["OK"] = { {InputType::keybd, KEY_INPUT_RETURN} ,
                             {InputType::pad,   PAD_INPUT_A} };    
    m_commandTable["cancel"] = { {InputType::keybd, KEY_INPUT_ESCAPE} ,
                             {InputType::pad,   PAD_INPUT_B} };     // 
    m_commandTable["pause"] = { {InputType::keybd,  KEY_INPUT_P},
                                {InputType::pad,    PAD_INPUT_R} }; // スタートボタン
    m_commandTable["keyconf"] = { {InputType::keybd,  KEY_INPUT_K},
                                {InputType::pad,    PAD_INPUT_L} }; // キーコンフィグ

    // ゲーム中関連
    // 見せるもの
    m_commandTable["dash"] = { {InputType::keybd,  KEY_INPUT_SPACE},
                                {InputType::pad,    PAD_INPUT_A} }; // キーコンフィグ

    // 見せないもの
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
    m_lastInputDate = m_inputDate;  // 直前入力をコピーしておく

    /* ハードウェア入力チェック */
    // キーボード用
    char keystate[256]; 
    GetHitKeyStateAll(keystate);   // 現在のキーボード入力を取得
    // パッド情報の取得
    int padstate = GetJoypadInputState(DX_INPUT_PAD1);  
    // スティック情報の取得
    int stickX, stickY;
    GetJoypadAnalogInput(&stickX, &stickY, DX_INPUT_PAD1);
    m_inputStickDate.x = static_cast<float>(stickX);
    m_inputStickDate.y = static_cast<float>(stickY);

    /*情報更新*/
    // 登録された情報とハードの情報を照らし合わせながら、
    // m_inputDataの内容を更新していく
    for (const auto& cmd : m_commandTable)
    {
        // コマンドの名前から入力データを作る
        auto& input = m_inputDate[cmd.first];

        if (cmd.first == "up" && stickY < 0)
        {
            input = true;
            continue;
        }
        if (cmd.first == "down" && stickY > 0)
        {
            input = true;
            continue;
        }
        if (cmd.first == "left" && stickX < 0)
        {
            input = true;
            continue;
        }
        if (cmd.first == "right" && stickX > 0)
        {
            input = true;
            continue;
        }

        for (const auto& hardIO : cmd.second)
        {
            input = false;
            // キーボードのチェック
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
