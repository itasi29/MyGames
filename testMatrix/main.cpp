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



	// 中心
	Vec3 center = Vec3();

	// 回転用
	float angle = 1;
	Vec3 axisVec = Vec3(1, 1, 1);

	// オブジェ用
	Vec3 posDir = Vec3(0, 0, 1);
	const float kPosDirDis = 30.0f;

	// カメラ用
	Vec3 cameraPos = Vec3(150, 50, 0);

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		// カメラの移動
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			cameraPos = AngleAxis(1, Vec3::Up()) * cameraPos;
		}
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			cameraPos = AngleAxis(1, Vec3::Up()) * cameraPos;
		}
		SetCameraPositionAndTarget_UpVecY(cameraPos.GetVECTOR(), VGet(0, 0, 0));

		// 360 / 60 = 6
		angle += 6.0f;
		// オブジェクト・軸の移動
		Vec3 trs;
		float move = 0.5f;
		if (CheckHitKey(KEY_INPUT_A))
		{
			trs.x -= move;
		}
		if (CheckHitKey(KEY_INPUT_D))
		{
			trs.x += move;
		}
		if (CheckHitKey(KEY_INPUT_W))
		{
			trs.z -= move;
		}
		if (CheckHitKey(KEY_INPUT_S))
		{
			trs.z += move;
		}
		center += trs;

		// オブジェクトの回転
		Quaternion rot = AngleAxis(angle, axisVec);
		Vec3 pos = center + rot * posDir * kPosDirDis;

		// 描画
		DrawSphere3D(pos.GetVECTOR(), 16, 16, 0xffffff, 0x000000, true);

		// 軸
		auto start = center;
		auto end = center + axisVec * 10;
		DrawCapsule3D(start.GetVECTOR(), end.GetVECTOR(), 8, 16, 0xffffff, 0x000000, true);
		// 座標用
		DrawLine3D(VGet(100, 0, 0), VGet(-100, 0, 0), 0xff0000);
		DrawLine3D(VGet(0, 100, 0), VGet(0, -100, 0), 0x00ff00);
		DrawLine3D(VGet(0, 0, 100), VGet(0, 0, -100), 0x0000ff);

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}