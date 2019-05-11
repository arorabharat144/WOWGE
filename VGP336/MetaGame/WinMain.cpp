#include <Engine/Inc/Engine.h>

#include "TestState.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WOWGE::Engine::GameApp myApp;

	myApp.GetStateMachine().AddState<TestState>();

	myApp.GetStateMachine().ChangeState(0);

	myApp.Initialize(hInstance, "MetaGame");

	while (myApp.IsRunning())
	{
		myApp.Update();
	}

	myApp.Terminate();
	return 0;
}