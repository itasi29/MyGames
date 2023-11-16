#include "Application.h"
#include "Scene/SceneManager.h"
#include "Common/Input.h"

#include <DxLib.h>
#include <cassert>

//現状は初めからゲームシーンにいってほしいから一時的にゲームシーンに飛ぶようにする
#include "Scene/GamePlayingScene.h"
#include "Scene/TitleScene.h"


namespace 
{
    // 画面サイズ
    constexpr int kScreenWidth = 1280;
    constexpr int kScreenHeight = 720;

    // FPS固定値
    constexpr int kFpsFrame = 16667;
}

int MyLoadGraph(const wchar_t* path)
{
    int handle = LoadGraph(path);
    assert(handle != -1);
    return handle;
}

Application::Application()
{
    m_windowSize = Size{kScreenWidth, kScreenHeight};
}

void Application::Terminate()
{
    DxLib_End();
}

bool Application::Init()
{
#ifdef _DEBUG
    ChangeWindowMode(true); // ウィンドウモードにします
#endif
    SetGraphMode(m_windowSize.w, m_windowSize.h, 16);
    SetWindowText(L"CircleRoom");
    if (DxLib_Init() == -1)
    {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    return true;
}

void Application::Run()
{
    SceneManager manager(this->GetInstance());
    // 一時的にゲームシーンスタートに
//    manager.ChangeScene(std::make_shared<TitleScene>(manager));
    manager.ChangeScene(std::make_shared<GamePlayingScene>(manager));

    Input input;
    while (ProcessMessage() != -1)
    {
        // 新しいゲームループを始めた時間を記憶
        m_time = GetNowHiPerformanceCount();

        ClearDrawScreen();
        input.Update(); // 入力を更新
        manager.Update(input);
        manager.Draw();
        ScreenFlip();

        // エスケープキーが押されたら終了する
        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }

        while (kFpsFrame > GetNowHiPerformanceCount() - m_time);
    }
    Terminate();
}

const Size& Application::GetWindowSize() const
{
    return m_windowSize;
}
