#include "GameApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameApp myApp;
	myApp.Initialize(hInstance, "Hello Triangle!");

	while (myApp.IsRunning())
	{
		myApp.Update();
	}

	myApp.Terminate();
	return 0;
}