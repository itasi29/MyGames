#include <DxLib.h>

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    ChangeWindowMode(true);

    if (DxLib_Init() == -1)
    {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    long long time;
    while (ProcessMessage() != -1) {
        time = GetNowHiPerformanceCount();

        ClearDrawScreen();
        ScreenFlip();

        // 60FPS�ɌŒ�
        while (16667 > GetNowHiPerformanceCount() - time);
    }

    DxLib_End();

	return 0;
}