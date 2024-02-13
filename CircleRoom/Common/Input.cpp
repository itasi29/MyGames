#include "Input.h"
#include <DxLib.h>
#include <cassert>
#include <string>
#include <algorithm>

using namespace std;

namespace
{
    constexpr float kVersion = 1.3f;

    struct KeyConfHeader
    {
        char id[4] = "kyc"; // �Ō��'\0'�����Ă�̂�4�o�C�g
        float version = kVersion;
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
    m_commandTable["OK"] = { {InputType::keybd, { KEY_INPUT_X } } ,
                             {InputType::pad,   { PAD_INPUT_A } } };
    m_commandTable["cancel"] = { {InputType::keybd, { KEY_INPUT_C } } ,
                             {InputType::pad,   { PAD_INPUT_B } } };     // 
    m_commandTable["pause"] = { {InputType::keybd,  { KEY_INPUT_ESCAPE } },
                                {InputType::pad,    { PAD_INPUT_R } } }; // �X�^�[�g�{�^��
    // �����Ȃ�����
    m_commandTable["optionLeft"] = { {InputType::keybd, { KEY_INPUT_Q } },
                                     {InputType::pad,   { PAD_INPUT_5 } } };
    m_commandTable["optionRight"] = { {InputType::keybd, { KEY_INPUT_E } },
                                     {InputType::pad,   { PAD_INPUT_6 } } };

    // �Q�[�����֘A
    // ���������
    m_commandTable["dash"] = { {InputType::keybd,  { KEY_INPUT_SPACE } },
                                {InputType::pad,    { PAD_INPUT_A } } }; // �L�[�R���t�B�O

    // �����Ȃ�����
    m_commandTable["up"] = { {InputType::keybd,  { KEY_INPUT_UP, KEY_INPUT_W } },
                             {InputType::pad,    { PAD_INPUT_UP } } };
    m_commandTable["down"] = { {InputType::keybd,  { KEY_INPUT_DOWN , KEY_INPUT_S }},
                             {InputType::pad,    { PAD_INPUT_DOWN } } };
    m_commandTable["left"] = { {InputType::keybd,  { KEY_INPUT_LEFT , KEY_INPUT_A }},
                             {InputType::pad,    { PAD_INPUT_LEFT  } } };
    m_commandTable["right"] = { {InputType::keybd,  { KEY_INPUT_RIGHT , KEY_INPUT_D }},
                             {InputType::pad,    { PAD_INPUT_RIGHT } } };

    m_exclusiveKeyConfigCommands = {"optionLeft", "optionRight", "up", "down", "left", "right"};

    Load(L"Data/Bin/key.cnf");

    auto& pad = m_corrTable[InputType::pad];
    pad[PAD_INPUT_A] = L"�`Bottan";
    pad[PAD_INPUT_B] = L"�aBottan";
    pad[PAD_INPUT_C] = L"�wBottan";
    pad[PAD_INPUT_X] = L"�xBottan";
    pad[PAD_INPUT_5] = L"�kBottan";
    pad[PAD_INPUT_6] = L"�qBottan";
    pad[PAD_INPUT_R] = L"�r�s�`�q�sBottan";
    pad[PAD_INPUT_L] = L"�lBottan";

    auto& keybd = m_corrTable[InputType::keybd];
    keybd[KEY_INPUT_A] = L"�`�L�[";
    keybd[KEY_INPUT_B] = L"�a�L�[";
    keybd[KEY_INPUT_C] = L"�b�L�[";
    keybd[KEY_INPUT_D] = L"�c�L�[";
    keybd[KEY_INPUT_E] = L"�d�L�[";
    keybd[KEY_INPUT_F] = L"�e�L�[";
    keybd[KEY_INPUT_G] = L"�f�L�[";
    keybd[KEY_INPUT_H] = L"�g�L�[";
    keybd[KEY_INPUT_I] = L"�h�L�[";
    keybd[KEY_INPUT_J] = L"�i�L�[";
    keybd[KEY_INPUT_K] = L"�j�L�[";
    keybd[KEY_INPUT_L] = L"�k�L�[";
    keybd[KEY_INPUT_M] = L"�l�L�[";
    keybd[KEY_INPUT_N] = L"�m�L�[";
    keybd[KEY_INPUT_O] = L"�n�L�[";
    keybd[KEY_INPUT_P] = L"�o�L�[";
    keybd[KEY_INPUT_Q] = L"�p�L�[";
    keybd[KEY_INPUT_R] = L"�q�L�[";
    keybd[KEY_INPUT_S] = L"�r�L�[";
    keybd[KEY_INPUT_T] = L"�s�L�[";
    keybd[KEY_INPUT_U] = L"�t�L�[";
    keybd[KEY_INPUT_V] = L"�u�L�[";
    keybd[KEY_INPUT_W] = L"�v�L�[";
    keybd[KEY_INPUT_X] = L"�w�L�[";
    keybd[KEY_INPUT_Y] = L"�x�L�[";
    keybd[KEY_INPUT_Z] = L"�y�L�[";
    keybd[KEY_INPUT_BACK] = L"BS�L�[";
    keybd[KEY_INPUT_TAB] = L"Tab�L�[";
    keybd[KEY_INPUT_RETURN] = L"Enter�L�[";
    keybd[KEY_INPUT_LSHIFT] = L"��Shift�L�[";
    keybd[KEY_INPUT_RSHIFT] = L"�EShift�L�[";
    keybd[KEY_INPUT_LCONTROL] = L"��Ctrl�L�[";
    keybd[KEY_INPUT_RCONTROL] = L"�ECtrl�L�[";
    keybd[KEY_INPUT_ESCAPE] = L"Esc�L�[";
    keybd[KEY_INPUT_SPACE] = L"�X�y�[�X�L�[";
}

void Input::Update()
{
    m_lastInputDate = m_inputDate;  // ���O���͂��R�s�[���Ă���
    m_isLastAnyPush = m_isAnyPush;

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
    else
    {
        m_isAnyPush = false;
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
                for (const auto& code : hardIO.second)
                {
                    if (keystate[code])
                    {
                        input = true;
                        m_lastType = hardIO.first;
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
                        m_lastType = hardIO.first;
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

std::wstring Input::GetHardDataName(const std::string cmd, InputType type) const
{
    return  m_corrTable.at(type).at(m_commandTable.at(cmd).at(type)[0]);
}

void Input::Save(const std::string& path)
{
    FILE* fp = nullptr; // �t�@�C���|�C���^
    auto err = fopen_s(&fp, path.c_str(), "wb");
    if (err != errno)
    {
        // �ǂݍ��݂Ɏ��s�������ߏI��
        assert(false);
        return;
    }
    // �w�b�_�̏�������
    KeyConfHeader header;
    header.dataCount = m_commandTable.size();
    fwrite(&header, sizeof(header), 1, fp);

    // �f�[�^�{�̂���������ł���
    for (const auto& cmd : m_commandTable)
    {
        const auto& commandStr = cmd.first; // �R�}���h������
        uint8_t m_size = static_cast<uint8_t>(commandStr.size());   // �R�}���h������̃T�C�Y���擾
        fwrite(&m_size, sizeof(m_size), 1, fp); // �R�}���h������̃o�C�g������������
        fwrite(commandStr.data(), commandStr.size(), 1, fp);    // ������̏�������

        const auto& InputData = cmd.second; // �Ή��������
        uint8_t inputTypeSize = static_cast<uint8_t>(InputData.size());   // �Ή�������͂̐�(��{�I��2, �L�[�{�[�h�ƃp�b�h)
        fwrite(&inputTypeSize, sizeof(inputTypeSize), 1, fp);   // 2����������
        for (const auto& input : InputData)
        {
            const auto& type = input.first; // �L�[�{�[�h���p�b�h��
            fwrite(&type, sizeof(type), 1, fp);

            // �T�C�Y�̓ǂݍ���
            uint8_t size = static_cast<uint8_t>(input.second.size());
            fwrite(&size, sizeof(size), 1, fp);

            for (const auto& state : input.second)
            {
                // ���ۂ̓���ID
                fwrite(&state, sizeof(state), 1, fp);
            }
        }
    }

    fclose(fp);
}

void Input::Load(const std::wstring& path)
{
    auto handle = FileRead_open(path.c_str());
    // �G���[(�t�@�C�����Ȃ�)�ꍇ�͏������Ȃ�
    if (handle == 0)
    {
        return;
    }
    // �w�b�_�̓ǂݍ���
    KeyConfHeader header;
    FileRead_read(&header, sizeof(header), handle);
    if (header.version != kVersion)
    {
        FileRead_close(handle);
        return;
    }

    // �f�[�^�̓ǂݍ���
    for (int i = 0; i < header.dataCount; i++)
    {
        uint8_t cmdStrSize = 0; // �R�}���h������̃T�C�Y
        FileRead_read(&cmdStrSize, sizeof(cmdStrSize), handle); // �R�}���h������T�C�Y�ǂݍ���
        std::string cmdStr; // �R�}���h������
        cmdStr.resize(cmdStrSize);  // �R�}���h������T�C�Y�Ń��T�C�Y
        FileRead_read(cmdStr.data(), static_cast<int>(cmdStr.size() * sizeof(char)), handle);   // �R�}���h������̓ǂݍ���

        auto& command = m_commandTable[cmdStr]; // �R�}���h�e�[�u������Ώۂ̃R�}���h�̎Q�Ƃ��擾

        uint8_t inputTypeSize;  // ���͎�ʐ����擾���Ă���(��{�I�ɂ�2�������Ă�)
        FileRead_read(&inputTypeSize, sizeof(inputTypeSize), handle);   // ��ʐ��̎擾
        for (int j = 0; j < inputTypeSize; j++) // �擾������ʐ��������[�v����
        {
            InputType type;
            uint8_t size;
            std::vector<int> state;

            FileRead_read(&type, sizeof(type), handle); // ��ʂ�ǂ݂���

            // �T�C�Y�ǂݍ���
            FileRead_read(&size, sizeof(size), handle);
            state.resize(size);

            for (int i = 0; i < size; i++)
            {
                // ���ۂ̓��̓X�e�[�g����ǂݍ���
                FileRead_read(&state[i], sizeof(state[i]), handle);
            }

            command[type] = state;  // �R�}���h�ɔ��f������
        }
    }

    FileRead_close(handle);
}
