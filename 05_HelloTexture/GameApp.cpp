#include "GameApp.h"
#include <Graphics/Inc/Gui.h>

using namespace WOWGE;

namespace
{

	//MIGHT HAVE TO CHANGE THE TANGENTS AND UV 
	//UV CAN BE CHANGED BASED ON THE FACE
	const Graphics::Vertex kVertices[] =
	{
		//CUBE

		//FRONT FACE
		/*0*/{ {0.0f, 10.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 2.0f} },
		/*1*/{ {10.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {2.0f, 0.0f} },
		/*2*/{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f},  {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
		/*3*/{ {10.0f, 10.0f, 0.0f}, {0.0f, 0.0f, -1.0f},{1.0f, 0.0f, 0.0f}, {2.0f, 2.0f} },

		//BACK FACE
		/*4*/{ {0.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 2.0f} },
		/*5*/{ {10.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {2.0f, 0.0f} },
		/*6*/{ {0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 1.0f},  {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
		/*7*/{ {10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 1.0f},{1.0f, 0.0f, 0.0f}, {2.0f, 2.0f} },

		//TOP FACE
		/*8*/{ {0.0f, 10.0f, 10.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f }, {0.0f, 2.0f} },
		/*9*/{ {10.0f, 10.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f }, {2.0f, 0.0f} },
		/*10*/{ {0.0f, 10.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f }, {0.0f, 0.0f} },
		/*11*/{ {10.0f, 10.0f, 10.0f},{0.0f, 1.0f, 0.0f},{0.0f, 0.0f, 1.0f }, {2.0f, 2.0f} },

		//BOTTOM FACE
		/*12*/{ {0.0f, 0.0f, 10.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f }, {0.0f, 2.0f} },
		/*13*/{ {10.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f }, {2.0f, 0.0f} },
		/*14*/{ {0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f},  {0.0f, 0.0f, 1.0f }, {0.0f, 0.0f} },
		/*15*/{ {10.0f, 0.0f, 10.0f}, {0.0f, -1.0f, 0.0f},{0.0f, 0.0f, 1.0f }, {2.0f, 2.0f} },

		//LEFT FACE
		/*16*/{ {0.0f, 10.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {2.0f, 0.0f} },
		/*17*/{ {0.0f, 0.0f, 10.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 2.0f} },
		/*18*/{ {0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
		/*19*/{ {0.0f, 10.0f, 10.0f}, {-1.0f, 0.0f, 0.0f},{0.0f, 1.0f, 0.0f}, {2.0f, 2.0f} },

		//RIGHT FACE
		/*20*/{ {10.0f, 10.0f, 0.0f}, {1.0f, 0.0f, 0.0f },{0.0f, 1.0f, 0.0f}, {2.0f, 0.0f} },
		/*21*/{ {10.0f, 0.0f, 10.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 2.0f} },
		/*22*/{ {10.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
		/*23*/{ {10.0f, 10.0f, 10.0f}, {1.0f, 0.0f, 0.0f},{0.0f, 1.0f, 0.0f}, {2.0f, 2.0f} }

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

		//TOP FACE
		8, 9, 10,
		8, 11, 9,

		//BOTTOM FACE
		12, 14, 13,
		12, 13, 15,

		//LEFT FACE
		16, 18, 17,
		16, 17, 19,

		//RIGHT FACE
		20, 21, 22,
		20, 23, 21
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

	mMeshBuffer.Initialize(kVertices, sizeof(Graphics::Vertex), kVertexCount, kIndices, kIndexCount);

	////////////////////////////////
	// Create and fill constant buffer
	mConstantBuffer.Initialize();

	mCubeTexture.Initialize("../Assets/Images/Metal.jpg");
	mWallTexture.Initialize("../Assets/Images/BrickSmallBrown.jpg");

	mPointSamplerUsingBorder.Initialize(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Border);
	mLinearSamplerUsingClamp.Initialize(Graphics::Sampler::Filter::Linear, Graphics::Sampler::AddressMode::Clamp);
	mAnisotropicSamplerUsingMirror.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Mirror);
	mAnisotropicSamplerUsingWrap.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	mVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", "VS", "vs_5_0", Graphics::Vertex::Format);

	///////////////////////////////////////
	// Compile and create the pixel shader

	mPixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");

	mCamera.Initialize(0.785f, 720, 720, 0.5f, 1000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mTimer.Initialize();
}

void GameApp::OnTerminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mAnisotropicSamplerUsingWrap.Terminate();
	mAnisotropicSamplerUsingMirror.Terminate();
	mLinearSamplerUsingClamp.Terminate();
	mPointSamplerUsingBorder.Terminate();
	mWallTexture.Terminate();
	mCubeTexture.Terminate();
	mConstantBuffer.Terminate();
	mMeshBuffer.Terminate();

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

	if (inputSystem->IsMouseDown(Input::MouseButton::RBUTTON))
	{
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
	}

	mCamera.UpdateViewMatrix();
	
	// Draw mesh

	// Bind the input layout, vertex shader, and pixel shader
	mVertexShader.Bind();
	mPixelShader.Bind();

	float cubeMover = 0.0f;

	Maths::Matrix44 world[10];
	for (uint32_t i = 0; i < 10; ++i)
	{
		if (i == 3)
		{
			cubeMover = 10.0f;
		}
		else if (i == 4)
		{
			cubeMover = 20.0f;
		}
		else if (i == 5)
		{
			cubeMover = 30.0f;
		}
		else if (i == 6)
		{
			cubeMover = 40.0f;
		}
		else if (i == 7)
		{
			cubeMover = 50.0f;
		}
		else if (i == 8)
		{
			cubeMover = 60.0f;
		}
		else if (i == 9)
		{
			cubeMover = 70.0f;
		}

		if (i < 3)
		{
			world[i] =
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				(float)i*20.0f, 0.0f, 5.0f, 1.0f
			};
		}
		if (i >= 3)
		{
			world[i] =
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				75.0f - cubeMover, 0.0f, 50.0f, 1.0f
			};
		}


		if (i >= 0 && i < 3)
		{
			mCubeTexture.BindPS(0);
			mCubeTexture.BindVS(0);
		}
		else
		{
			mWallTexture.BindPS(0);
			mWallTexture.BindVS(0);
		}

		if (i == 0)
		{
			mPointSamplerUsingBorder.BindVS(0);
			mPointSamplerUsingBorder.BindPS(0);
		}
		else if (i == 1)
		{
			mLinearSamplerUsingClamp.BindVS(0);
			mLinearSamplerUsingClamp.BindPS(0);
		}
		else if (i == 2)
		{
			mAnisotropicSamplerUsingMirror.BindVS(0);
			mAnisotropicSamplerUsingMirror.BindPS(0);
		}
		else
		{
			mAnisotropicSamplerUsingWrap.BindVS(0);
			mAnisotropicSamplerUsingWrap.BindPS(0);
		}

		// Update matrices
		TransformData data;
		data.world = Maths::Transpose(world[i]);
		data.view = Maths::Transpose(mCamera.GetViewMatrix());
		data.projection = Maths::Transpose(mCamera.GetProjectionMatrix());

		mConstantBuffer.Set(data);
		mConstantBuffer.BindVS();
		mMeshBuffer.Render();
	}

	graphicsSystem->EndRender();
}