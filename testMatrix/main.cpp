#include <DxLib.h>
#include "Common/Geometry.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	Vec3 start(10, 10, 0);
	Vec3 end(110, 110, 0);
	float t = 0.0f;

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		auto easeIn = Easing::EaseIn(start, end, t);

		t += 0.01f;
		if (t >= 1.0f)
		{
			t = 0.0f;
		}

		DrawCircle(easeIn.x, easeIn.y, 8, 0xffffff, true);

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}