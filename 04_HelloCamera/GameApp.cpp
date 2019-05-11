#include "GameApp.h"

#include <Graphics/Inc/Gui.h>

using namespace WOWGE;

namespace
{
	const Graphics::VertexPC kVertices[] =
	{
		//CUBE

		//FRONT FACE
		{ Maths::Vector3(0.0f, 0.5f, 0.0f), Maths::Vector4::Red() }, //0
		{ Maths::Vector3(0.5f, 0.0f, 0.0f), Maths::Vector4::Green() }, //1
		{ Maths::Vector3(0.0f, 0.0f, 0.0f), Maths::Vector4::Blue() }, //2
		{ Maths::Vector3(0.5f, 0.5f, 0.0f), Maths::Vector4::Yellow() }, //3

		//BACK FACE
		{ Maths::Vector3(0.0f, 0.5f, 0.5f), Maths::Vector4::Red() }, //4
		{ Maths::Vector3(0.5f, 0.0f, 0.5f), Maths::Vector4::Green() }, //5
		{ Maths::Vector3(0.0f, 0.0f, 0.5f), Maths::Vector4::Blue() }, //6
		{ Maths::Vector3(0.5f, 0.5f, 0.5f), Maths::Vector4::Yellow() }, //7
	};
	constexpr int kVertexCount = sizeof(kVertices) / sizeof(kVertices[0]);

	//FOR INDEX ORDER OF RENDER
	const uint32_t kIndices[] =
	{
		//FRONT FACE
		0, 1, 2,
		0, 3, 1,

		//BACK FACE
		4, 6, 5,
		4, 5, 7,

		//LEFT FACE
		4, 2, 6,
		4, 0, 2,

		//RIGHT FACE
		3, 5, 1,
		3, 7, 5,

		//TOP FACE
		4, 3, 0,
		4, 7, 3,

		//BOTTOM FACE
		6, 2, 1,
		6, 1, 5
	};
	constexpr int kIndexCount = sizeof(kIndices) / sizeof(kIndices[0]);

	float speed = 10.0f;
	float rotationSpeed = 10.0f;
}

GameApp::GameApp()
{
	
}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), 720, 720);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	mMeshBuffer.Initialize(kVertices, sizeof(Graphics::VertexPC), kVertexCount, kIndices, kIndexCount);

	////////////////////////////////
	// Create and fill constant buffer
	mConstantBuffer.Initialize();

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	mVertexShader.Initialize(L"../Assets/Shaders/DoTransform.fx", "VS", "vs_5_0", Graphics::VertexPC::Format);

	///////////////////////////////////////
	// Compile and create the pixel shader

	mPixelShader.Initialize(L"../Assets/Shaders/DoTransform.fx");

	mCamera.Initialize(0.785f, 720, 720, 1.0f, 1000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mTimer.Initialize();
}

void GameApp::OnTerminate()
{

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();

	Graphics::GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();
	Gui::Terminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	mTimer.Update();
	float deltaTime = mTimer.GetElapsedTime();
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


	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	auto inputSystem = Input::InputSystem::Get();
	graphicsSystem->BeginRender();

	if (inputSystem->IsKeyDown(Input::KeyCode::W))
	{
		mCamera.Walk(speed * deltaTime);
	}
	if (inputSystem->IsKeyDown(Input::KeyCode::S))
	{
		mCamera.Walk(-speed * deltaTime);
	}
	if (inputSystem->IsKeyDown(Input::KeyCode::A))
	{
		mCamera.Strafe(-speed * deltaTime);
	}
	if (inputSystem->IsKeyDown(Input::KeyCode::D))
	{
		mCamera.Strafe(speed * deltaTime);
	}
	if (inputSystem->IsKeyDown(Input::KeyCode::Q))
	{
		mCamera.Rise(-speed * deltaTime);
	}
	if (inputSystem->IsKeyDown(Input::KeyCode::E))
	{
		mCamera.Rise(speed * deltaTime);
	}

	mCamera.Pitch(rotationSpeed * deltaTime * inputSystem->GetMouseMoveY()); //MOUSE MOVEMENT
	mCamera.Yaw(rotationSpeed * deltaTime * inputSystem->GetMouseMoveX()); //MOUSE MOVEMENT

	mCamera.UpdateViewMatrix();
	
	// Draw mesh

	// Bind the input layout, vertex shader, and pixel shader
	mVertexShader.Bind();
	mPixelShader.Bind();


	Maths::Matrix44 world[10][10];

	for (uint32_t i = 0; i < 10; ++i)
	{
		for (uint32_t j = 0; j < 10; ++j)
		{
			world[i][j] =
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				(float)j-5.0f, -2.5f, (float)i+10.0f, 1.0f
			};
			// Update matrices
			TransformData data;
			data.world = Maths::Transpose(world[i][j]);
			data.view = Maths::Transpose(mCamera.GetViewMatrix());
			data.projection = Maths::Transpose(mCamera.GetProjectionMatrix());

			mConstantBuffer.Set(data);
			mConstantBuffer.BindVS();
			mMeshBuffer.Render();
		}
	}

	graphicsSystem->EndRender();
}