#include "Application.h"
#include <DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	Application& application = Application::GetInstance();

	if (!application.Init())
	{
		// ���̃A�v���͕s���I�����܂����B
		return -1;
	}
	application.Run();
	application.Terminate();

	// ���̃A�v���͐���I�����܂����B
	return 0;
}