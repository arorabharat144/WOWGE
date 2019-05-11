#include "GameApp.h"

#include <Graphics/Inc/Gui.h>

using namespace WOWGE;

GameApp::GameApp()
{

}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), 1920, 1080);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	mCamera.Initialize(0.785f, 720.0f, 720.0f, 0.5f, 100000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.SetPosition({ 0.0f, 0.0f, -3000.0f });
	mTimer.Initialize();

	Audio::AudioSystem::StaticInitialize();
	Audio::SoundEffectController::StaticInitialize();

	Audio::SoundEffectController::Get()->Load("record-scratch_zkkU_2VO.wav");
	Audio::SoundEffectController::Get()->Load("live-the-moment_z1XWyUBO.wav");
}

void GameApp::OnTerminate()
{
	Graphics::SimpleDraw::Terminate();
	Graphics::GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();
	Gui::Terminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	mTimer.Update();
	float deltaTime = mTimer.GetElapsedTime();
	float totalTime = mTimer.GetTotalTime();
	if (mWindow.ProcessMessage())
	{
		Kill();
		return;
	}

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}
	Input::InputSystem::Get()->Update();

	auto inputSystem = Input::InputSystem::Get();

	Audio::AudioSystem::Get()->Update();

	if (inputSystem->IsKeyPressed(Input::KeyCode::ONE))
	{
		Audio::SoundEffectController::Get()->Play(0);
	}

	Audio::SoundEffectController::Get()->Play(1, true);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	graphicsSystem->BeginRender();

	mCamera.UpdateViewMatrix();

	graphicsSystem->EndRender();

}