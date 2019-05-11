#include "GameApp.h"

#include <Graphics/Inc/Gui.h>

using namespace WOWGE;

namespace
{
	float speed = 1000.0f;
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

	Graphics::MeshBuilder::CreateSphere(mSphereMesh, 200, 200);
	mSphereMeshBuffer.Initialize(mSphereMesh.GetVertices(), sizeof(Graphics::Vertex), mSphereMesh.GetVertexCount(), mSphereMesh.GetIndices(), mSphereMesh.GetIndexCount());

	Graphics::MeshBuilder::CreateSkySphere(mSkySphere, 100, 100);
	mSkySphereMeshBuffer.Initialize(mSkySphere.GetVertices(), sizeof(Graphics::Vertex), mSkySphere.GetVertexCount(), mSkySphere.GetIndices(), mSkySphere.GetIndexCount());

	////////////////////////////////
	// Create and fill constant buffer
	mConstantBuffer.Initialize();

	mLight.ambient = Maths::Vector4::White();
	mLight.diffuse = Maths::Vector4::White();
	mLight.direction = { 0.0f, 0.0f, 1.0f, 0.0f };
	mLight.specular = Maths::Vector4::White();

	mMaterial.ambient = Maths::Vector4::White();
	mMaterial.diffuse = Maths::Vector4::White();
	mMaterial.emissive = Maths::Vector4::White();
	mMaterial.specular = Maths::Vector4::White();
	mMaterial.power = 10.0f;

	mDiffuseTexture.Initialize("../Assets/Images/Planets/Earth.jpg");
	mSpecularTexture.Initialize("../Assets/Images/Planets/EarthSpecular.jpg");
	mDisplacementTexture.Initialize("../Assets/Images/Planets/EarthDisplacement.jpg");
	mNormalTexture.Initialize("../Assets/Images/Planets/EarthNormal.jpg");

	mAnisotropicSamplerUsingWrap.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	mVertexShader.Initialize(L"../Assets/Shaders/Earth.fx", "VS", "vs_5_0", Graphics::Vertex::Format);

	///////////////////////////////////////
	// Compile and create the pixel shader

	mPixelShader.Initialize(L"../Assets/Shaders/Earth.fx");

	mCamera.Initialize(0.785f, 720.0f, 720.0f, 0.5f, 100000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.SetPosition({ 0.0f, 0.0f, -3000.0f });
	mTimer.Initialize();
}

void GameApp::OnTerminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mAnisotropicSamplerUsingWrap.Terminate();

	mDiffuseTexture.Terminate();
	mSpecularTexture.Terminate();
	mDisplacementTexture.Terminate();
	mNormalTexture.Terminate();

	mSphereMesh.Destroy();
	mSkySphere.Destroy();

	mConstantBuffer.Terminate();
	mSphereMeshBuffer.Terminate();
	mSkySphereMeshBuffer.Terminate();
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


	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	auto inputSystem = Input::InputSystem::Get();
	graphicsSystem->BeginRender();

	if (inputSystem->IsMouseDown(Input::MouseButton::RBUTTON))
	{
		auto boost = inputSystem->IsKeyDown(Input::KeyCode::LSHIFT) ? 10.0f : 1.0f;
		if (inputSystem->IsKeyDown(Input::KeyCode::W))
		{
			mCamera.Walk(speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::S))
		{
			mCamera.Walk(-speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::A))
		{
			mCamera.Strafe(-speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::D))
		{
			mCamera.Strafe(speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::Q))
		{
			mCamera.Rise(-speed * boost * deltaTime);
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::E))
		{
			mCamera.Rise(speed * boost * deltaTime);
		}

		mCamera.Pitch(rotationSpeed * deltaTime * inputSystem->GetMouseMoveY()); //MOUSE MOVEMENT
		mCamera.Yaw(rotationSpeed * deltaTime * inputSystem->GetMouseMoveX()); //MOUSE MOVEMENT
	}

	mCamera.UpdateViewMatrix();

	// Draw mesh

	// Bind the input layout, vertex shader, and pixel shader
	mVertexShader.Bind();
	mPixelShader.Bind();

	//THIS IS SKYSPHERE

	Maths::Matrix44 skySphereWorldMatrix = Maths::Matrix44::Scaling(50000.0f) * Maths::Matrix44::Translation(mCamera.GetPosition().x, mCamera.GetPosition().y, mCamera.GetPosition().z);
	Maths::Matrix44 wvpSkySphere = skySphereWorldMatrix * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();

	mDiffuseTexture.BindPS(0);
	mDiffuseTexture.BindVS(0);

	mSpecularTexture.BindPS(1);

	mDisplacementTexture.BindVS(2);

	mAnisotropicSamplerUsingWrap.BindVS(0);
	mAnisotropicSamplerUsingWrap.BindPS(0);

	planetTranslation = { 2041.0f, 0.0f, 0.0f };
	planetScaling = { 100.0f, 100.0f, 100.0f };

	float hack = 5.0f;
	float speed = 10.0f;
	WOWGE::Maths::Matrix44 planetWorldMatrix =
		Maths::Matrix44::Scaling(planetScaling.x * hack, planetScaling.y * hack, planetScaling.z * hack) *
		Maths::Matrix44::Translation(planetTranslation.x, planetTranslation.y, planetTranslation.z);

	mDiffuseTexture.BindPS(0);
	mSpecularTexture.BindPS(1);
	mDisplacementTexture.BindVS(2);
	mNormalTexture.BindPS(3);

	TransformData planetsData;
	planetsData.world = Maths::Transpose(planetWorldMatrix);
	planetsData.wvp = Maths::Transpose(planetWorldMatrix * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix());
	planetsData.cameraPosition = { mCamera.GetPosition().x, mCamera.GetPosition().y, mCamera.GetPosition().z, 0.0f };
	planetsData.lightDirection = { mCamera.GetDirection().x, mCamera.GetDirection().y, mCamera.GetDirection().z, 0.0f };
	planetsData.lightAmbient = mLight.ambient;
	planetsData.lightDiffuse = mLight.diffuse;
	planetsData.lightSpecular = mLight.specular;
	planetsData.materialAmbient = mMaterial.ambient;
	planetsData.materialDiffuse = mMaterial.diffuse;
	planetsData.materialSpecular = mMaterial.specular;
	planetsData.materialEmissive = mMaterial.emissive;
	planetsData.materialPower = mMaterial.power;
	planetsData.displacementFactor = 0.1f;

	mConstantBuffer.Set(planetsData);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mSphereMeshBuffer.Render();

	graphicsSystem->EndRender();

}