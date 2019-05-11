#include "Precompiled.h"
#include "GameApp.h"

#include "MetaRegistration.h"

#include "ResourceManager.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

namespace
{
	float speed{ 50.0f };
	float rotationSpeed{ 10.0f };
}

GameApp::GameApp() :
	mStateMachine{ *this }
{

}
GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	Engine::Meta::MetaRegister();

	uint32_t width{ 1920 };
	uint32_t height{ 1080 };
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);

	mTimer.Initialize();

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	Graphics::SimpleDraw::Initialize();

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	mWorld.Initialize(100);

	mWorld.LoadLevel("../../Assets/Templates/Level_01.xml");
}

void GameApp::OnTerminate()
{
	mWorld.Terminate();

	mStateMachine.Purge();
	Input::InputSystem::StaticTerminate();
	Graphics::SimpleDraw::Terminate();
	Graphics::GraphicsSystem::StaticTerminate();
	Gui::Terminate();

	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	if (mWindow.ProcessMessage())
	{
		Kill();
		return;
	}

	Input::InputSystem* inputSystem{ Input::InputSystem::Get() };
	Graphics::GraphicsSystem* graphicsSystem{ Graphics::GraphicsSystem::Get() };

	inputSystem->Update();
	mTimer.Update();

	if (inputSystem->IsKeyPressed(Input::KeyCode::ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}
	float deltaTime{ mTimer.GetElapsedTime() };

	mStateMachine.Update(deltaTime);

	graphicsSystem->BeginRender();

	mWorld.Update(deltaTime);
	mWorld.Render();

	//Graphics::SimpleDraw::Render(mCamera);

	graphicsSystem->EndRender();
}