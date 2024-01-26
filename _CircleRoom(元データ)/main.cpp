#include "Application.h"
#include <DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	Application& application = Application::GetInstance();

	if (!application.Init())
	{
		return -1;// このアプリは不正終了しました。
	}
	application.Run();

	// このアプリは正常終了しました。
	return 0;
}