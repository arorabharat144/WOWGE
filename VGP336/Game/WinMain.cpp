#include <Engine/Inc/Engine.h>

#include "HelloSimpleDraw.h"
#include "HelloAudio.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WOWGE::Engine::GameApp myApp;

	myApp.GetStateMachine().AddState<HelloSimpleDraw>();
	myApp.GetStateMachine().AddState<HelloAudio>();

	myApp.GetStateMachine().ChangeState(0);

	myApp.Initialize(hInstance, "Game");

	while (myApp.IsRunning())
	{
		myApp.Update();
	}

	myApp.Terminate();
	return 0;
}