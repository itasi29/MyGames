#include <DxLib.h>
#include "MyEngine/Geometry.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	MyEngine::Vec3 start = MyEngine::Vec3(0, 0, 0);
	MyEngine::Vec3 end   = MyEngine::Vec3(100, 100, 0);

	float t = 0;

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		t += 0.005f;
		if (t > 1.0f) t = 0.0f;

		MyEngine::Vec3 linear    = MyEngine::Vec3(20, 50, 0) + MyEngine::Easing::Linear(start, end, t);
		MyEngine::Vec3 easeIn    = MyEngine::Vec3(120, 50, 0) + MyEngine::Easing::EaseIn(start, end, t);
		MyEngine::Vec3 easeOut   = MyEngine::Vec3(220, 50, 0) + MyEngine::Easing::EaseOut(start, end, t);
		MyEngine::Vec3 easeInOut = MyEngine::Vec3(320, 50, 0) + MyEngine::Easing::EaseInOut(start, end, t);

		DrawFormatString(16, 16, 0xffffff, L"t = %.3f", t);

		DrawCircle(linear.x, linear.y, 5, 0xffffff, true);
		DrawCircle(easeIn.x, easeIn.y, 5, 0xff0000, true);
		DrawCircle(easeOut.x, easeOut.y, 5, 0x00ff00, true);
		DrawCircle(easeInOut.x, easeInOut.y, 5, 0x0000ff, true);

		DrawLine(0, 50, 640, 50, 0xff00ff);
		DrawLine(0, 150, 640, 150, 0xff00ff);

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}