#include "Application.h"
#include <DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	Application& application = Application::GetInstance();

	if (!application.Init())
	{
		// このアプリは不正終了しました。
		return -1;
	}
	application.Run();
	application.Terminate();

	// このアプリは正常終了しました。
	return 0;
}