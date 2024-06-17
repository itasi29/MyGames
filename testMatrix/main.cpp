#include <DxLib.h>
#include "Common/Geometry.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	Vec3 start = Vec3(100, 50, 0);
	Vec3 end   = Vec3(200, 150, 0);

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		Vec3 pos = ;
		DrawCircle();

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}