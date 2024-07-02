#include <DxLib.h>
#include <cassert>
#include "MyEngine/Geometry/Geometry.h"
#include "MyEngine/DebugDraw.h"
#include "MyEngine/Physics/Physics.h"
#include "Common/Input.h"
#include "Player.h"
#include "Enemy.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

#if false
	SetCameraPositionAndTarget_UpVecY(VGet(0, 300, -500), VGet(0, 200, 0));
	SetCameraNearFar(1.0f, 10000.0f);
#else
	SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -50), VGet(0, 0, 0));
	SetCameraNearFar(1.0f, 200.0f);
#endif

	auto& debugDraw = MyEngine::DebugDraw::GetInstance();

	auto& input = Input::GetInstance();

	auto& physics = MyEngine::Physics::GetInstance();
	auto player = std::make_shared<Player>();
	auto enemy = std::make_shared<Enemy>();
	physics.Entry(player);
	physics.Entry(enemy);

	MyEngine::Matrix4x4 modelMat = MyEngine::Matrix4x4();
	float angle = 0.0f;
	modelMat.Identity();
	modelMat.SetPos(MyEngine::Vec3(0, 10, 0));
	int modelH = MV1LoadModel(L"Model/player.mv1");
	assert(modelH != -1);

	MyEngine::Vec3 start = MyEngine::Vec3(0, 0, 0);
	MyEngine::Vec3 end   = MyEngine::Vec3(100, 100, 0);
	float t = 0;

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();
		debugDraw.Clear();
//		debugDraw.Gulid();

		input.Update(0);

		/* Physicsお試し */
		{
			physics.Update();

			player->Update();
		}

		/* Geometryお試し */
		{
#if false
			angle += 0.3f;
			MyEngine::Quaternion rotPos = MyEngine::AngleAxis(2.0f, MyEngine::Vec3(1.0f, 1.0f, 0.0f));
			modelMat.SetPos(rotPos * modelMat.GetPos());
			modelMat.SetRot(MyEngine::AngleAxis(angle, MyEngine::Vec3(0, 1, 0.5f)));
			MV1SetMatrix(modelH, modelMat.GetMATRIX());
			MV1DrawModel(modelH);
#endif
		}

		/* イージングお試し */
		{
#if false
			t += 0.005f;
			if (t > 1.0f) t = 0.0f;

			MyEngine::Vec3 linear = MyEngine::Lerp(start, end, t);
			MyEngine::Vec3 easeIn = MyEngine::EaseIn(start, end, t);
			MyEngine::Vec3 easeOut = MyEngine::EaseOut(start, end, t);
			MyEngine::Vec3 easeInOut = MyEngine::EaseInOut(start, end, t);

			debugDraw.DrawCircle({ linear, 5, 0xffffff });
			debugDraw.DrawCircle({ easeIn, 5, 0xff0000 });
			debugDraw.DrawCircle({ easeOut, 5, 0x00ff00 });
			debugDraw.DrawCircle({ easeInOut, 5, 0x0000ff });
			debugDraw.DrawLine({ MyEngine::Vec3(0, 50, 0), MyEngine::Vec3(640, 50, 0), 0xff00ff });
			debugDraw.DrawLine({ MyEngine::Vec3(0, 150, 0), MyEngine::Vec3(640, 150, 0), 0xff00ff });
			DrawFormatString(16, 32, 0xff00ff, L"t = %.3f", t);
#endif
		}

		debugDraw.Draw();

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}