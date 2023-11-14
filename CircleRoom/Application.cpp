#include "Application.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Common/Input.h"

#include <DxLib.h>
#include <cassert>

namespace 
{
    constexpr int kScreenWidth = 1280;
    constexpr int kScreenHeight = 720;
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
    SetGraphMode(m_windowSize.w, m_windowSize.h, 1);
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
    manager.ChangeScene(std::make_shared<TitleScene>(manager));
    Input input;
    while (ProcessMessage() != -1)
    {
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
    }
    Terminate();
}

const Size& Application::GetWindowSize() const
{
    return m_windowSize;
}
