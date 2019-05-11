#include <Engine/Inc/Engine.h>

#include "GameObjectUser.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WOWGE::Engine::GameApp myApp;

	myApp.Initialize(hInstance, "GameObjectUsage");

	myApp.GetStateMachine().AddState<GameObjectUser>();

	myApp.GetStateMachine().ChangeState(0);
	while (myApp.IsRunning())
	{
		myApp.Update();
	}

	myApp.Terminate();
	return 0;
}