#include <DxLib.h>
#include <memory>
#include "Geometry.h"
#include "Camera.h"
#include "Obj/Cube.h"
#include "Obj/Sphere.h"
#include "Obj/Caspsule.h"

#define CUBE_
#define SPHERE_
#define CASPSULE_
#define CASPSULE2

void DrawGrid();

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    ChangeWindowMode(true);

    if (DxLib_Init() == -1)
    {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

	SetGlobalAmbientLight(GetColorF(1.0f, 1.0f, 1.0f, 0.0f));

	Camera camera;
	camera.Init();

#ifdef CUBE
    std::shared_ptr<Cube> cube1 = std::make_shared<Cube>();
    std::shared_ptr<Cube> cube2 = std::make_shared<Cube>();
    cube1->Init(Pos3(), Size(10, 5, 10));
	cube2->Init(Pos3(10, 10, 0), Size(8, 16, 10));
#endif
#ifdef SPHERE
	std::shared_ptr<Sphere> sphere1 = std::make_shared<Sphere>();
	std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>();
	sphere1->Init(Pos3(), 4);
	sphere2->Init(Pos3(10, 10, 0), 8);
#endif
#ifdef CASPSULE
	std::shared_ptr<Caspsule> caspsule = std::make_shared<Caspsule>();
	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>();
	caspsule->Init(Pos3(), Vec3(0, 1, 0), 4, 5);
	sphere->Init(Pos3(10, 10, 0), 4);
#endif
#ifdef CASPSULE2
	std::shared_ptr<Caspsule> caspsule1 = std::make_shared<Caspsule>();
	std::shared_ptr<Caspsule> caspsule2 = std::make_shared<Caspsule>();
	caspsule1->Init(Pos3(), Vec3(1, 0.5f, 0), 3, 2);
	caspsule2->Init(Pos3(), Vec3(0, 1, 1), 4, 3);
#endif

	bool isMoveObj = true;
	bool isPush = false;

    long long time;
    while (ProcessMessage() != -1) {
        time = GetNowHiPerformanceCount();

        ClearDrawScreen();

		if (CheckHitKey(KEY_INPUT_RETURN))
		{
			if (!isPush)
			{
				isMoveObj = !isMoveObj;
				isPush = true;
			}
		}
		else
		{
			isPush = false;
		}

		camera.Update(!isMoveObj);

		DrawGrid();

#ifdef CUBE
        cube1->Update();
		cube2->Update(isMoveObj);
		cube2->IsHit(cube1->GetCol());

        cube1->Draw();
		cube2->Draw();
#endif
#ifdef SPHERE
		sphere1->Update();
		sphere2->Update(isMoveObj);
		sphere2->IsHit(sphere1->GetCol());

		sphere1->Draw();
		sphere2->Draw();
#endif
#ifdef CASPSULE
		caspsule->Update();
		sphere->Update(isMoveObj);
		caspsule->IsHit(sphere->GetCol());

		caspsule->Draw();
		sphere->Draw();
#endif
#ifdef CASPSULE2
		caspsule1->Update();
		caspsule2->Update(isMoveObj);
		caspsule2->IsHit(caspsule1->GetCol());

		caspsule1->Draw();
		caspsule2->Draw();
#endif
		if (isMoveObj)
		{
			DrawString(10, 10, L"MoveObj", 0xffffff);
		}
		else
		{
			DrawString(10, 10, L"MoveCamera", 0xffffff);
		}

        ScreenFlip();

        // 60FPSに固定
        while (16667 > GetNowHiPerformanceCount() - time);
    }

    DxLib_End();

	return 0;
}

void DrawGrid()
{
	for (int x = -50; x <= 50; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -50), VGet(static_cast<float>(x), 0, 50), 0xffff00);
	}
	for (int z = -50; z <= 50; z += 10)
	{
		DrawLine3D(VGet(-50, 0, static_cast<float>(z)), VGet(50, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-の方向が分かりやすいように表示を追加する
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(50, 0, 0));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, L"X+", 0xffffff);
	}

	dispPos = ConvWorldPosToScreenPos(VGet(-50, 0, 0));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, L"X-", 0xffffff);
	}

	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 50));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, L"Z+", 0xffffff);
	}

	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -50));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, L"Z-", 0xffffff);
	}
}