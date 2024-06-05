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

	float angle = 30;
	float rad = angle * Math::kDeg2Rad;

	// ‰ñ“]¨ˆÚ“®


	// DXLIB‚Ì•û‚Æ‚ ‚Á‚Ä‚¢‚é‚©Šm”F
	MATRIX rotZMat = MGetRotZ(rad);
	MATRIX trsMat = MGetTranslate(VGet(10, 0, 0));
	MATRIX ansMat = MMult(rotZMat, trsMat);

	Quaternion rot = AngleAxis(angle, Vec3(0, 1, 0));
	Matrix4x4 trs = Move(Vec3(10, 0, 0));
	Matrix4x4 ans = rot * trs;
	Matrix4x4 inverse = ans.Inverse(true);
	
	Matrix4x4 ans2 = ans * inverse;


	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}