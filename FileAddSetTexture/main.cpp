#include <DxLib.h>
#include <vector>
#include <memory>
#include "File/FileBase.h"
#include "File/FileManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	SetCameraPositionAndTarget_UpVecY(VGet(-500, 100, -200), VGet(0, 0, 0));

	FileManager mgr;
	mgr.Init();

	std::vector<std::shared_ptr<FileBase>> file;
	file.push_back(mgr.Load(L"mechaTex1"));
	file.push_back(mgr.Load(L"mechaTex2"));
	file.push_back(mgr.Load(L"mechaTex3"));
	file.push_back(mgr.Load(L"mecha", false));

	int modelH = MV1DuplicateModel(file[3]->GetHandle());
	mgr.SetTexture(modelH, file[3]->GetName());

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		MV1DrawModel(modelH);

		ScreenFlip();
	}

	MV1DeleteModel(modelH);

	mgr.End();

	DxLib_End();

	return 0;
}