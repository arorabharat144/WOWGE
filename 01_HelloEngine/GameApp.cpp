#include "GameApp.h"

using namespace WOWGE;

GameApp::GameApp()
{

}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), 1280, 720);
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
}

void GameApp::OnTerminate()
{
	Graphics::GraphicsSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	if (mWindow.ProcessMessage())
	{
		Kill();
		return;
	}

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();

	

	graphicsSystem->EndRender();
}