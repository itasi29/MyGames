#include "Application.h"
#include <DxLib.h>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) 
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