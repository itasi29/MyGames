#include "Input.h"
#include <DxLib.h>

namespace
{
    // パッド使用データ
    constexpr int kPadData[] =
    {
        DX_INPUT_PAD1,
        DX_INPUT_PAD2,
        DX_INPUT_PAD3,
        DX_INPUT_PAD4,
        DX_INPUT_PAD5,
        DX_INPUT_PAD6,
        DX_INPUT_PAD7,
        DX_INPUT_PAD8,
    };
}

Input::Input()
{
    /* コマンドテーブル作成 */
    m_commandTable["ok"] = { { InputType::Key, KEY_INPUT_RETURN },
                             { InputType::Pad, PAD_INPUT_A } };
    m_commandTable["up"] = { { InputType::Key, KEY_INPUT_UP },
                             { InputType::Pad, PAD_INPUT_UP } };
    m_commandTable["down"] = { { InputType::Key, KEY_INPUT_DOWN },
                               { InputType::Pad, PAD_INPUT_DOWN } };
    m_commandTable["right"] = { { InputType::Key, KEY_INPUT_RIGHT },
                                { InputType::Pad, PAD_INPUT_RIGHT } };
    m_commandTable["left"] = { { InputType::Key, KEY_INPUT_LEFT },
                               { InputType::Pad, PAD_INPUT_LEFT } };
}

Input::~Input()
{
}

Input& Input::GetInstance()
{
    static Input instance;
    return instance;
}

void Input::Update()
{
    InputLog_t newLog;

    bool isFirst = true;
    char keystate[256];
    int padstate;
    // Key情報取得
    GetHitKeyStateAll(keystate);

    // 最大Pad数分繰り返す
    for (int i = 0; i < Game::kMaxPadNum; ++i)
    {
        // 各パッド情報取得
        padstate = GetJoypadInputState(kPadData[i]);

        // コマンド分繰り返す
        for (const auto& cmd : m_commandTable)
        {
            // 参照取得
            bool& isInput = newLog[i][cmd.first];
            isInput = false;

            for (auto hardIO : cmd.second)
            {
                if (hardIO.first == InputType::Pad)
                {
                    // Pad判定
                    if (padstate & hardIO.second)
                    {
                        isInput = true;
                        break;
                    }
                }
                // 1PのみKeyを反映させる
                else if (isFirst && hardIO.first == InputType::Key)
                {
                    // Key判定
                    if (keystate[hardIO.second])
                    {
                        isInput = true;
                        break;
                    }
                }
            }
        }

        // 1P終了とする
        isFirst = false;
    }

    // ログの更新
    for (int i = kLogNum - 1; i > 0; --i)
    {
        // HACK: こんなキャストの仕方の方がいいらしいのでこうする
        m_log[i] = m_log[static_cast<std::array<InputLog_t, 16Ui64>::size_type>(i) - 1];
    }
    m_log[0] = newLog;
}

bool Input::IsTrigger(const std::string& cmd, int no) const
{
    return m_log.at(0).at(no).at(cmd) && !m_log.at(1).at(no).at(cmd);
}

bool Input::IsPress(const std::string& cmd, int no) const
{
    return m_log.at(0).at(no).at(cmd);
}

bool Input::IsRelease(const std::string& cmd, int no) const
{
    return !m_log.at(0).at(no).at(cmd) && m_log.at(1).at(no).at(cmd);
}
