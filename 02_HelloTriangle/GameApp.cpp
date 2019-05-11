#include "GameApp.h"

using namespace WOWGE;



namespace
{
	enum class FigureType
	{
		TRIFORCE,
		HEART,
		STAR,
		TRANSFORMER
	};

	FigureType figureType = FigureType::TRIFORCE;

	struct SimpleVertex
	{
		Maths::Vector3 position;
		Maths::Vector4 color;
	};

	const SimpleVertex kTriforceVertices[] =
	{
		//TRIFORCE

		//TRIANGLE 1
		/*TOP VERTEX*/{ Maths::Vector3(0.0f, 1.0f, 0.0f), Maths::Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.25f, 0.25f, 0.0f), Maths::Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(-0.25f, 0.25f, 0.0f), Maths::Vector4(1.0f, 1.0f, 0.0f, 1.0f) },

		//TRIANGLE 2
		/*TOP VERTEX*/{ Maths::Vector3(-0.25f, 0.25f, 0.0f), Maths::Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.0f, -0.5f, 0.0f), Maths::Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(-0.5f, -0.5f, 0.0f), Maths::Vector4(1.0f, 1.0f, 0.0f, 1.0f) },

		//TRIANGLE 3
		/*TOP VERTEX*/{ Maths::Vector3(0.25f, 0.25f, 0.0f), Maths::Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.5f, -0.5f, 0.0f), Maths::Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(0.0f, -0.5f, 0.0f), Maths::Vector4(1.0f, 1.0f, 0.0f, 1.0f) },

	};
	const int kTriforceVertexCount = sizeof(kTriforceVertices) / sizeof(kTriforceVertices[0]);

	const SimpleVertex kHeartVertices[] =
	{
		//HEART

		//TRIANGLE 1
		/*TOP VERTEX*/{ Maths::Vector3(0.0f, 0.5f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.25f, 0.0f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) }, 
		/*LEFT VERTEX*/{ Maths::Vector3(-0.25f, 0.0f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) }, 

		//TRIANGLE 2
		/*TOP VERTEX*/{ Maths::Vector3(-0.25f, 0.5f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.0f, 0.5f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(-0.25f, 0.0f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) }, 

		//TRIANGLE 3
		/*TOP VERTEX*/ { Maths::Vector3(0.0f, 0.5f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.25f, 0.5f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(0.25f, 0.0f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) }, 

		//TRIANGLE 4
		/*TOP VERTEX*/{ Maths::Vector3(-0.125f, 0.75f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.0f, 0.5f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(-0.25f, 0.5f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },

		//TRIANGLE 5
		/*TOP VERTEX*/{ Maths::Vector3(0.125f, 0.75f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.25f, 0.5f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(0.0f, 0.5f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },

		//TRIANGLE 6
		/*TOP VERTEX*/ { Maths::Vector3(-0.25f, 0.0f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) }, 
		/*RIGHT VERTEX*/{ Maths::Vector3(0.25f, 0.0f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) }, 
		/*LEFT VERTEX*/{Maths::Vector3(0.0f, -0.25f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) }
	};
	const int kHeartVertexCount = sizeof(kHeartVertices) / sizeof(kHeartVertices[0]);

	const SimpleVertex kStarVertices[] =
	{
		//STAR

		//TRIANGLE 1
		/*TOP VERTEX*/{ Maths::Vector3(0.0f, 0.75f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.25f, 0.0f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(-0.25f, 0.0f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },

		//TRIANGLE 2
		/*TOP VERTEX*/{ Maths::Vector3(-0.25f, 0.5f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.25f, 0.5f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(0.0f, -0.25f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) }
	};
	const int kStarVertexCount = sizeof(kStarVertices) / sizeof(kStarVertices[0]);

	const SimpleVertex kTransformerVertices[] =
	{
		//TRANSFORMER

		//TRIANGLE 1
		/*TOP VERTEX*/{ Maths::Vector3(0.0f, 0.35f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.25f, -0.25f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(-0.25f, -0.25f, 0.0f), Maths::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },

		//TRIANGLE 2
		/*TOP VERTEX*/{ Maths::Vector3(-0.35f, 0.35f, 0.0f), Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.0f, 0.35f, 0.0f), Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(-0.25f, -0.25f, 0.0f), Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f) },

		//TRIANGLE 3
		/*TOP VERTEX*/{ Maths::Vector3(0.0f, 0.35f, 0.0f), Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.35f, 0.35f, 0.0f), Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(0.25f, -0.25f, 0.0f), Maths::Vector4(0.0f, 1.0f, 0.0f, 1.0f) },

		//TRIANGLE 4
		/*TOP VERTEX*/{ Maths::Vector3(0.0f, 0.65f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.15f, 0.35f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(-0.15f, 0.35f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },

		//TRIANGLE 5
		/*TOP VERTEX*/{ Maths::Vector3(-0.25f, -0.25f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(-0.15f, -0.75f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(-0.5f, -0.75f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },

		//TRIANGLE 6
		/*TOP VERTEX*/{ Maths::Vector3(0.25f, -0.25f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*RIGHT VERTEX*/{ Maths::Vector3(0.45f, -0.75f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		/*LEFT VERTEX*/{ Maths::Vector3(0.15f, -0.75f, 0.0f), Maths::Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
	};

	const int kTransformerVertexCount = sizeof(kTransformerVertices) / sizeof(kTransformerVertices[0]);
}

GameApp::GameApp():
	mVertexBufferTriforce(nullptr),
	mVertexBufferHeart(nullptr)
{
	
}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), 1280, 720);
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	////////////////////////////////
	// Create and fill vertex buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * kTriforceVertexCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	//FOR TRIFORCE
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = kTriforceVertices;

	graphicsSystem->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mVertexBufferTriforce);


	//FOR HEART
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * kHeartVertexCount;
	initData.pSysMem = kHeartVertices;

	graphicsSystem->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mVertexBufferHeart);


	//FOR STAR
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * kStarVertexCount;
	initData.pSysMem = kStarVertices;

	graphicsSystem->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mVertexBufferStar);

	//FOR TRANSFORMER
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * kTransformerVertexCount;
	initData.pSysMem = kTransformerVertices;

	graphicsSystem->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mVertexBufferTransformer);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	mVertexShader.Initialize(L"../Assets/Shaders/DoColor.fx", "VS", "vs_5_0", Graphics::VertexPC::Format);

	///////////////////////////////////////
	// Compile and create the pixel shader

	mPixelShader.Initialize(L"../Assets/Shaders/DoColor.fx");
}

void GameApp::OnTerminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	SafeRelease(mVertexBufferTriforce);
	SafeRelease(mVertexBufferHeart);
	SafeRelease(mVertexBufferStar);
	SafeRelease(mVertexBufferTransformer);

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


	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;


	// Set primitive topology
	graphicsSystem->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw mesh
	if (GetAsyncKeyState(VK_F1))
	{
		figureType = FigureType::TRIFORCE;
	
	}
	else if (GetAsyncKeyState(VK_F2))
	{
		figureType = FigureType::HEART;

	}
	else if (GetAsyncKeyState(VK_F3))
	{
		figureType = FigureType::STAR;
	}
	else if (GetAsyncKeyState(VK_F4))
	{
		figureType = FigureType::TRANSFORMER;
	}


	// Bind the input layout, vertex shader, and pixel shader
	mVertexShader.Bind();
	mPixelShader.Bind();

	if (figureType == FigureType::TRIFORCE)
	{
		graphicsSystem->GetContext()->IASetVertexBuffers(0, 1, &mVertexBufferTriforce, &stride, &offset);
		graphicsSystem->GetContext()->Draw(kTriforceVertexCount, 0);
	}
	else if (figureType == FigureType::HEART)
	{
		graphicsSystem->GetContext()->IASetVertexBuffers(0, 1, &mVertexBufferHeart, &stride, &offset);
		graphicsSystem->GetContext()->Draw(kHeartVertexCount, 0);
	}
	else if (figureType == FigureType::STAR)
	{
		graphicsSystem->GetContext()->IASetVertexBuffers(0, 1, &mVertexBufferStar, &stride, &offset);
		graphicsSystem->GetContext()->Draw(kStarVertexCount, 0);
	}
	else if (figureType == FigureType::TRANSFORMER)
	{
		graphicsSystem->GetContext()->IASetVertexBuffers(0, 1, &mVertexBufferTransformer, &stride, &offset);
		graphicsSystem->GetContext()->Draw(kTransformerVertexCount, 0);
	}

	graphicsSystem->EndRender();
}