#include <DxLib.h>
#include "Application.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Application& app = Application::GetInstance();
	if (!app.Init()) {
		return false;
	}
	app.Run();
	app.Terminate();

	return 0;				// ソフトの終了
}