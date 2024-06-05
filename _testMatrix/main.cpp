#include <DxLib.h>
#include "Common/Geometry.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}