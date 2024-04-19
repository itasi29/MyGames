#include "Application.h"
#include <DxLib.h>
#include "Game.h"
#include "Geometry.h"

namespace
{
    const wchar_t* const kWindowText = L"TempGame";
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

    if (DxLib_Init() == -1) {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);

    return true;
}

void Application::Run()
{
    auto cameraPos = VGet(0.0f, 10.0f, -100.0f);
    auto targetPos = VGet(0.0f, 0.0f, 0.0f);

    SetCameraNearFar(0.1f, 1000.0f);
    SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

    int playerHandle = MV1LoadModel(L"Data/Model/player.mv1");
    int enemyHandle = MV1LoadModel(L"Data/Model/enemy.mv1");

    constexpr float kPlayerScale = 0.125f;
    constexpr float kEnemyScale = 20.0f;

    MV1SetScale(playerHandle, VGet(kPlayerScale, kPlayerScale, kPlayerScale));
    MV1SetScale(enemyHandle, VGet(kEnemyScale, kEnemyScale, kEnemyScale));

    Pos3 playerPos(50, -10.0f, 0.0f);
    Pos3 enemyPos(-50.0f, -10.0f, 0.0f);

    constexpr float kColRadius = 2.0f;

    CircleCol playerCol;
    CircleCol enemyCol;

    playerCol.SetRadisu(kColRadius);
    enemyCol.SetRadisu(kColRadius);


    long long time;
    while (ProcessMessage() != -1) {
        time = GetNowHiPerformanceCount();

        ClearDrawScreen();

        constexpr float kSpeed = 0.1f;
        if (CheckHitKey(KEY_INPUT_RIGHT))
        {
            playerPos.x += kSpeed;
        }
        if (CheckHitKey(KEY_INPUT_LEFT))
        {
            playerPos.x -= kSpeed;
        }

        MV1SetPosition(playerHandle, playerPos.GetVECTOR());
        MV1SetPosition(enemyHandle, enemyPos.GetVECTOR());

        playerCol.SetPos(playerPos);
        enemyCol.SetPos(enemyPos);

        MV1DrawModel(playerHandle);
        MV1DrawModel(enemyHandle);
        playerCol.Draw();
        if (playerCol.IsHit(enemyCol))
        {
            enemyCol.Draw(0x00ff00);
        }

        ScreenFlip();

        if (m_isEnd) {
            break;
        }
        while (Game::kFpsFrame > GetNowHiPerformanceCount() - time);
    }

    MV1DeleteModel(playerHandle);
    MV1DeleteModel(enemyHandle);
}

void Application::Terminate()
{
    DxLib_End();
}
