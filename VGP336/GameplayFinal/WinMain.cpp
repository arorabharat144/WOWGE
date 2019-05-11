#include <Engine/Inc/Engine.h>

#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WOWGE::Engine::GameApp myApp;

	myApp.Initialize(hInstance, "Game");

	myApp.GetStateMachine().AddState<Game>();

	myApp.GetStateMachine().ChangeState(0);
	while (myApp.IsRunning())
	{
		myApp.Update();
	}

	myApp.Terminate();
	return 0;
}