#include <Engine/Inc/Engine.h>

#include "HelloSprint.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WOWGE::Engine::GameApp myApp;

	myApp.GetStateMachine().AddState<HelloSprint>();

	myApp.GetStateMachine().ChangeState(0);

	myApp.Initialize(hInstance, "HelloSprint");

	if (lpCmdLine != nullptr)
	{
		HelloSprint::Host();
	}
	else
	{
		HelloSprint::Join();
	}
	while (myApp.IsRunning())
	{
		myApp.Update();
	}

	myApp.Terminate();

	return 0;
}