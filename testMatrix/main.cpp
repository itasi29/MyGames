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

	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);

	// カメラ位置
	SetCameraPositionAndTarget_UpVecY(VGet(50, 50, -150), VGet(0, 0, 0));
	// ライト
	int light = CreateDirLightHandle(VGet(-1, 1, -1));



	float angle = 1;
	Vec3 axisVec = Vec3(1, 1, 1);
	Vec3 axisCenter = Vec3(0, 0, 0);

	// オブジェ用
	Matrix4x4 mat;
	mat.SetPos(Vec3(-30, 30, 30));

	// カメラ用
	Vec3 cameraPos = Vec3(150, 50, 0);

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		// カメラの移動
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			cameraPos = AngleAxis(angle, Vec3::Up()) * cameraPos;
		}
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			cameraPos = AngleAxis(-angle, Vec3::Up()) * cameraPos;
		}
		SetCameraPositionAndTarget_UpVecY(cameraPos.GetVECTOR(), VGet(0, 0, 0));



		// オブジェクト・軸の移動
		Vec3 trs;
		float move = 0.5f;
		if (CheckHitKey(KEY_INPUT_A))
		{
			axisCenter.x -= move;
			trs.x -= move;
		}
		if (CheckHitKey(KEY_INPUT_D))
		{
			axisCenter.x += move;
			trs.x += move;
		}
		if (CheckHitKey(KEY_INPUT_W))
		{
			axisCenter.z -= move;
			trs.z -= move;
		}
		if (CheckHitKey(KEY_INPUT_S))
		{
			axisCenter.z += move;
			trs.z += move;
		}
		// オブジェクトの回転
		Quaternion rot = AngleAxis(angle, axisVec);
		mat = Move(trs) * rot * mat;
		// MEMO: なんか違う



		// 描画
		DrawSphere3D(mat.GetPos().GetVECTOR(), 16, 16, 0xffffff, 0x000000, true);

		// 軸
		DrawCapsule3D(axisCenter.GetVECTOR(), (axisCenter + axisVec * 10).GetVECTOR(), 8, 16, 0xffffff, 0x000000, true);
		// 座標用
		DrawLine3D(VGet(100, 0, 0), VGet(-100, 0, 0), 0xff0000);
		DrawLine3D(VGet(0, 100, 0), VGet(0, -100, 0), 0x00ff00);
		DrawLine3D(VGet(0, 0, 100), VGet(0, 0, -100), 0x0000ff);

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}