#include "Application.h"
#include <DxLib.h>
#include "Game.h"

namespace
{
    const wchar_t* const kWindowText = L"ShotTogether";
}

Application::Application() :
    m_isEnd(false),
#ifdef _DEBUG
    m_isWindows(true)
#else
    m_isWindows(false)
#endif
{
}

Application& Application::GetInstance()
{
    static Application app;
    return app;
}

bool Application::Init()
{
    ChangeWindowMode(m_isWindows);
    SetChangeScreenModeGraphicsSystemResetFlag(false);

    SetGraphMode(Game::kWindowWidth, Game::kWindowHeight, 32);
    SetWindowText(kWindowText);

    if (DxLib_Init() == -1)
    {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    return true;
}

void Application::Run()
{
    long long time;
    while (ProcessMessage() != -1) {
        time = GetNowHiPerformanceCount();

        ClearDrawScreen();
        ScreenFlip();

        if (m_isEnd) 
        {
            break;
        }
        while (Game::kFpsFrame > GetNowHiPerformanceCount() - time);
    }
}

void Application::Terminate()
{
    DxLib_End();
}
