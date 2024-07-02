#include "Application.h"
#include <DxLib.h>
#include "Game.h"
#include "Input.h"
#include "File/FileManager.h"

namespace
{
    const wchar_t* const kWindowText = L"BuumeranFu";
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

    SetGraphMode(Game::kWindowW, Game::kWindowH, 32);
    SetWindowText(kWindowText);

    if (DxLib_Init() == -1)
    {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);

    return true;
}

void Application::Run()
{
    auto& fileMgr = FileManager::GetInstance();
    auto& input = Input::GetInstance();

    long long time;
    while (ProcessMessage() != -1) 
    {
        time = GetNowHiPerformanceCount();

        ClearDrawScreen();

        for (int i = 0; i < Game::kMaxPlayNum; ++i)
        {
            input.Update(i);
        }
#ifdef _DEBUG

#else

#endif


        ScreenFlip();

        if (m_isEnd) 
        {
            break;
        }
        while (Game::kFPS > GetNowHiPerformanceCount() - time);
    }
}

void Application::Terminate()
{
    DxLib_End();
}
