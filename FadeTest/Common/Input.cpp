#include "Input.h"
#include <DxLib.h>

namespace
{
    // �p�b�h�g�p�f�[�^
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
    /* �R�}���h�e�[�u���쐬 */
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
    // Key���擾
    GetHitKeyStateAll(keystate);

    // �ő�Pad�����J��Ԃ�
    for (int i = 0; i < Game::kMaxPadNum; ++i)
    {
        // �e�p�b�h���擾
        padstate = GetJoypadInputState(kPadData[i]);

        // �R�}���h���J��Ԃ�
        for (const auto& cmd : m_commandTable)
        {
            // �Q�Ǝ擾
            bool& isInput = newLog[i][cmd.first];
            isInput = false;

            for (auto hardIO : cmd.second)
            {
                if (hardIO.first == InputType::Pad)
                {
                    // Pad����
                    if (padstate & hardIO.second)
                    {
                        isInput = true;
                        break;
                    }
                }
                // 1P�̂�Key�𔽉f������
                else if (isFirst && hardIO.first == InputType::Key)
                {
                    // Key����
                    if (keystate[hardIO.second])
                    {
                        isInput = true;
                        break;
                    }
                }
            }
        }

        // 1P�I���Ƃ���
        isFirst = false;
    }

    // ���O�̍X�V
    for (int i = kLogNum - 1; i > 0; --i)
    {
        // HACK: ����ȃL���X�g�̎d���̕��������炵���̂ł�������
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
