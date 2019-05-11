#include "GameApp.h"
#include <Graphics/Inc/Gui.h>

using namespace WOWGE;

namespace
{
	float speed = 50.0f;
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

	//Creating a cube of unit size
	Graphics::MeshBuilder::CreateCube(mCubeMesh);
	Graphics::MeshBuilder::CreateSphere(mSphereMesh, 20, 20);

	mCubeMeshBuffer.Initialize(mCubeMesh.GetVertices(), sizeof(Graphics::Vertex), mCubeMesh.GetVertexCount(), mCubeMesh.GetIndices(), mCubeMesh.GetIndexCount());
	mSphereMeshBuffer.Initialize(mSphereMesh.GetVertices(), sizeof(Graphics::Vertex), mSphereMesh.GetVertexCount(), mSphereMesh.GetIndices(), mSphereMesh.GetIndexCount());

	mTerrain.Initialize("../Assets/Heightmaps/heightmap.raw", 256, 256, 50.0f);
	////////////////////////////////
	// Create and fill constant buffer
	mConstantBuffer.Initialize();

	mLight.ambient = Maths::Vector4::White();
	mLight.diffuse = Maths::Vector4::White();
	mLight.direction = { 0.0f, -1.0f, -1.0f, 0.0f };
	mLight.specular = Maths::Vector4::White();

	mMaterial.ambient = { 0.75f, 0.25f, 0.125f, 1.0f };
	mMaterial.diffuse = { 0.75f, 0.25f, 0.125f, 1.0f };
	mMaterial.emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
	mMaterial.specular = { 0.125f, 0.125f, 0.125f, 1.0f };
	mMaterial.power = 2.0f;

	mTexture.Initialize("../Assets/Images/Ground.jpg");
	mAnisotropicSamplerUsingWrap.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	mVertexShader.Initialize(L"../Assets/Shaders/Lighting.fx", "VS", "vs_5_0", Graphics::Vertex::Format);

	///////////////////////////////////////
	// Compile and create the pixel shader

	mPixelShader.Initialize(L"../Assets/Shaders/Lighting.fx");

	mCamera.Initialize(0.785f, 720, 720, 0.5f, 1000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mTimer.Initialize();
}

void GameApp::OnTerminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mAnisotropicSamplerUsingWrap.Terminate();
	mTexture.Terminate();
	mConstantBuffer.Terminate();
	mCubeMeshBuffer.Terminate();
	mSphereMeshBuffer.Terminate();
	mTerrain.Terminate();
	mCubeMesh.Destroy();
	mSphereMesh.Destroy();


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

	Maths::Matrix44 world =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 10.0f, 1.0f
	};

	Maths::Matrix44 wvp = world * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();
	
	mTexture.BindPS(0);
	mTexture.BindVS(0);

	mAnisotropicSamplerUsingWrap.BindVS(0);
	mAnisotropicSamplerUsingWrap.BindPS(0);
		

	// Update matrices
	TransformData data;
	data.world = Maths::Transpose(world);
	data.wvp = Maths::Transpose(wvp);
	data.cameraPosition = { mCamera.GetPosition().x, mCamera.GetPosition().y, mCamera.GetPosition().z, 0.0f };
	data.lightDirection = mLight.direction;
	data.lightAmbient = mLight.ambient;
	data.lightDiffuse = mLight.diffuse;
	data.lightSpecular = mLight.specular;
	data.materialAmbient = mMaterial.ambient;
	data.materialDiffuse = mMaterial.diffuse;
	data.materialSpecular = mMaterial.specular;
	data.materialEmissive = mMaterial.emissive;
	data.materialPower = mMaterial.power;

	uint32_t sizeOfData = sizeof(data);

	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	//mCubeMeshBuffer.Render();
	mSphereMeshBuffer.Render();
	mTerrain.Render();

	graphicsSystem->EndRender();
}