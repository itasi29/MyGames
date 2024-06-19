#include <DxLib.h>
#include "Common/Input.h"
#include "Common/Game.h"
#include "Scene/SceneManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);
	SetGraphMode(Game::kWindowWidth, Game::kWindowHeight, 32);
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	auto& scnMgr = SceneManager::GetInstance();
	scnMgr.Init();
	auto& input = Input::GetInstance();

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		input.Update();

		scnMgr.Update();
		scnMgr.Draw();

		ScreenFlip();
	}

	scnMgr.Final();

	DxLib_End();

	return 0;
}