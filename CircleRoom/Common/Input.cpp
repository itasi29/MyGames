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
        char id[4] = "kyc"; // 最後に'\0'入ってるので4バイト
        float version = 1.0f;
        size_t dataCount = 0;
        // 空白の4バイト(パディング)
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

Input::Input()
{
    // メニュー関連
    // 見せるもの
    m_commandTable["OK"] = { {InputType::keybd, KEY_INPUT_RETURN} ,
                             {InputType::pad,   PAD_INPUT_A} };    
    m_commandTable["cancel"] = { {InputType::keybd, KEY_INPUT_ESCAPE} ,
                             {InputType::pad,   PAD_INPUT_B} };     // 
    m_commandTable["pause"] = { {InputType::keybd,  KEY_INPUT_P},
                                {InputType::pad,    PAD_INPUT_R} }; // スタートボタン
    m_commandTable["keyconf"] = { {InputType::keybd,  KEY_INPUT_K},
                                {InputType::pad,    PAD_INPUT_L} }; // キーコンフィグ
    // 見せないもの
    m_commandTable["optionLeft"] = { {InputType::keybd, KEY_INPUT_Q},
                                     {InputType::pad,   PAD_INPUT_5} };
    m_commandTable["optionRight"] = { {InputType::keybd, KEY_INPUT_E},
                                     {InputType::pad,   PAD_INPUT_6} };

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

    m_exclusiveKeyConfigCommands = {"optionLeft", "optionRight", "up", "down", "left", "right"};

    Load(L"Data/Bin/key.cnf");
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

void Input::Save(const std::string& path)
{
    FILE* fp = nullptr; // ファイルポインタ
    auto err = fopen_s(&fp, path.c_str(), "wb");
    // ヘッダの書き込み
    KeyConfHeader header;
    header.dataCount = m_commandTable.size();
    fwrite(&header, sizeof(header), 1, fp);

    // データ本体を書き込んでいく
    for (const auto& cmd : m_commandTable)
    {
        const auto& commandStr = cmd.first; // コマンド文字列
        uint8_t m_size = static_cast<uint8_t>(commandStr.size());   // コマンド文字列のサイズを取得
        fwrite(&m_size, sizeof(m_size), 1, fp); // コマンド文字列のバイト数を書き込む
        fwrite(commandStr.data(), commandStr.size(), 1, fp);    // 文字列の書き込み

        const auto& InputData = cmd.second; // 対応する入力
        uint8_t inputTypeSize = static_cast<uint8_t>(InputData.size());   // 対応する入力の数(基本的に2, キーボードとパッド)
        fwrite(&inputTypeSize, sizeof(inputTypeSize), 1, fp);   // 2を書き込む
        for (const auto& input : InputData)
        {
            const auto& type = input.first; // キーボードかパッドか
            const auto& state = input.second;   // 実際の入力ID

            fwrite(&type, sizeof(type), 1, fp);
            fwrite(&state, sizeof(state), 1, fp);
        }
    }

    fclose(fp);
}

void Input::Load(const std::wstring& path)
{
    auto handle = FileRead_open(path.c_str());
    // エラー(ファイルがない)場合は処理しない
    if (handle == 0)
    {
        return;
    }
    // ヘッダの読み込み
    KeyConfHeader header;
    FileRead_read(&header, sizeof(header), handle);

    // データの読み込み
    for (int i = 0; i < header.dataCount; i++)
    {
        uint8_t cmdStrSize = 0; // コマンド文字列のサイズ
        FileRead_read(&cmdStrSize, sizeof(cmdStrSize), handle); // コマンド文字列サイズ読み込み
        std::string cmdStr; // コマンド文字列
        cmdStr.resize(cmdStrSize);  // コマンド文字列サイズでリサイズ
        FileRead_read(cmdStr.data(), static_cast<int>(cmdStr.size() * sizeof(char)), handle);   // コマンド文字列の読み込み

        auto& command = m_commandTable[cmdStr]; // コマンドテーブルから対象のコマンドの参照を取得

        uint8_t inputTypeSize;  // 入力種別数を取得してくる(基本的には2が入ってる)
        FileRead_read(&inputTypeSize, sizeof(inputTypeSize), handle);   // 種別数の取得
        for (int j = 0; j < inputTypeSize; j++) // 取得した種別数だけループする
        {
            InputType type;
            int state;

            FileRead_read(&type, sizeof(type), handle); // 種別を読みこむ
            FileRead_read(&state, sizeof(state), handle);   // 実際の入力ステート情報を読み込む

            command[type] = state;  // コマンドに反映させる
        }
    }

    FileRead_close(handle);
}
