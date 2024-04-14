#include "Application.h"

#include <DxLib.h>
#include "Game.h"
#include "File/FileManager.h"
#include "ImageNumber.h"

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
    SetWindowText(L"SlimeGame3D");

    if (DxLib_Init() == -1) {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    return true;
}

void Application::Run()
{
    FileManager fileMgr;
    ImageNumber& imgNum = ImageNumber::GetInstance(fileMgr);

    long long time;
    while (ProcessMessage() != -1) {
        time = GetNowHiPerformanceCount();

        ClearDrawScreen();

        DrawLine(0, 100, 1280, 100, 0xffff00);
        DrawLine(100, 0, 100, 720, 0x00ffff);

        imgNum.Draw(100, 100, 14);
        imgNum.Draw(100, 100, 28, 1.0, DrawPos::LeftDown);
        imgNum.Draw(100, 228, 35, 2.0);
        imgNum.Draw(100, 296, 28, 1.0, DrawPos::RightUp);

        ScreenFlip();

        if (m_isEnd) {
            break;
        }

        while (Game::kFpsFrame > GetNowHiPerformanceCount() - time);
    }

    imgNum.End();
}

void Application::Terminate()
{
    DxLib_End();
}
