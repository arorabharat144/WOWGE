#include "GameApp.h"

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

	float rotationAroundY = 0.0f;
	float rotationAroundX = 0.0f;
}

GameApp::GameApp():
	mConstantBuffer(nullptr)
{
	
}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), 720, 720);
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	mMeshBuffer.Initialize(kVertices, sizeof(Graphics::VertexPC), kVertexCount, kIndices, kIndexCount);

	////////////////////////////////
	// Create and fill constant buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Maths::Matrix44) * 3;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;


	graphicsSystem->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &mConstantBuffer);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	mVertexShader.Initialize(L"../Assets/Shaders/DoTransform.fx", "VS", "vs_5_0", Graphics::VertexPC::Format);

	///////////////////////////////////////
	// Compile and create the pixel shader

	mPixelShader.Initialize(L"../Assets/Shaders/DoTransform.fx");
}

void GameApp::OnTerminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	SafeRelease(mConstantBuffer);

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
		return;
	}



	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();

	if (GetAsyncKeyState(VK_UP))
	{
		rotationAroundX -= 0.01f;
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		rotationAroundX += 0.01f;
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		rotationAroundY += 0.01f;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		rotationAroundY -= 0.01f;
	}

	// Draw mesh

	// Bind the input layout, vertex shader, and pixel shader
	mVertexShader.Bind();
	mPixelShader.Bind();


	Maths::Matrix44 world, view, projection;
	world = Maths::Matrix44::RotationX(rotationAroundX) * Maths::Matrix44::RotationY(rotationAroundY);
	view =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 3.0f, 1.0f
	};
	projection =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 0.0f
	};

	// Update matrices
	Maths::Matrix44 matrices[3];
	matrices[0] = Maths::Transpose(world);
	matrices[1] = Maths::Transpose(view);
	matrices[2] = Maths::Transpose(projection);

	graphicsSystem->GetContext()->UpdateSubresource(mConstantBuffer, 0, nullptr, matrices, 0, 0);
	graphicsSystem->GetContext()->VSSetConstantBuffers(0, 1, &mConstantBuffer);

	mMeshBuffer.Render(); 

	graphicsSystem->EndRender();
}