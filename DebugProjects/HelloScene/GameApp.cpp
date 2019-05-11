#include "GameApp.h"

using namespace WOWGE;

namespace
{
	float speed = 1000.0f;
	float rotationSpeed = 10.0f;
}

GameApp::GameApp():
	mCameraNode(nullptr),
	mLightNode(nullptr),
	mMaterialNode(nullptr),
	mMeshNode(nullptr),
	mSamplerNode(nullptr),
	mShaderNode(nullptr),
	mDiffuseTextureNode(nullptr),
	mSpecularTextureNode(nullptr),
	mDisplacementTextureNode(nullptr),
	mTransformNode(nullptr)
{
	mSceneManager = new WOWGE::Graphics::SceneManager(mMatrixStack);
}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), 720, 720);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	Graphics::MeshBuilder::CreateSphere(mSphereMesh, 200, 200);
	mSphereMeshBuffer.Initialize(mSphereMesh);

	Graphics::MeshBuilder::CreateSkySphere(mSkySphere, 100, 100);
	mSkySphereMeshBuffer.Initialize(mSkySphere);

	////////////////////////////////
	// Create and fill constant buffer

	//mLight.ambient = Maths::Vector4::White();
	//mLight.diffuse = Maths::Vector4::White();
	//mLight.direction = { 0.0f, 0.0f, 1.0f, 0.0f };
	//mLight.specular = Maths::Vector4::White();
	//
	//mMaterial.ambient = Maths::Vector4::White();
	//mMaterial.diffuse = Maths::Vector4::White();
	//mMaterial.emissive = Maths::Vector4::White();
	//mMaterial.specular = Maths::Vector4::White();
	//mMaterial.power = 10.0f;

	mDiffuseTexture.Initialize("../../Assets/Images/Planets/Earth.jpg");
	mSpecularTexture.Initialize("../../Assets/Images/Planets/EarthSpecular.jpg");
	mDisplacementTexture.Initialize("../../Assets/Images/Planets/EarthDisplacement.jpg");
	mNormalTexture.Initialize("../../Assets/Images/Planets/EarthNormal.jpg");

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	mVertexShader.Initialize(L"../../Assets/Shaders/Standard.fx", "VS", "vs_5_0", Graphics::Vertex::Format);

	///////////////////////////////////////
	// Compile and create the pixel shader

	mPixelShader.Initialize(L"../../Assets/Shaders/Standard.fx");

	mCamera.Initialize(0.785f, 720.0f, 720.0f, 0.5f, 100000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.SetPosition({ 0.0f, 0.0f,-10.0f });
	mTimer.Initialize();

	mCameraNode = mSceneManager->CreateCameraNode();
	mCameraNode->SetId(0);
	mCameraNode->SetName("Camera");
	mCameraNode->SetCamera(&mCamera);

	mLightNode = mSceneManager->CreateLightNode();
	mLightNode->SetId(1);
	mLightNode->SetName("Light");
	mLightNode->GetLight().ambient = Maths::Vector4::Black();
	mLightNode->GetLight().diffuse = Maths::Vector4::White();
	mLightNode->GetLight().direction = { 0.0f, 0.0f, 1.0f, 0.0f };
	mLightNode->GetLight().specular = Maths::Vector4::Black();

	mMaterialNode = mSceneManager->CreateMaterialNode();
	mMaterialNode->SetId(2);
	mMaterialNode->SetName("Material");
	mMaterialNode->GetMaterial().ambient = Maths::Vector4::Black();
	mMaterialNode->GetMaterial().diffuse = Maths::Vector4::White();
	mMaterialNode->GetMaterial().emissive = Maths::Vector4::Black();
	mMaterialNode->GetMaterial().specular = Maths::Vector4::Black();
	mMaterialNode->GetMaterial().power = 10.0f;

	mMeshNode = mSceneManager->CreateMeshNode();
	mMeshNode->SetId(3);
	mMeshNode->SetName("Mesh");
	mMeshNode->SetMesh(&mSphereMeshBuffer);

	mSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSamplerNode->SetId(4);
	mSamplerNode->SetName("Sampler");

	mShaderNode = mSceneManager->CreateShaderNode();
	mShaderNode->SetId(5);
	mShaderNode->SetName("Shader");
	mShaderNode->SetPixelShader(&mPixelShader);
	mShaderNode->SetVertexShader(&mVertexShader);

	mDiffuseTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mDiffuseTextureNode->SetId(6);
	mDiffuseTextureNode->SetName("DiffuseTexture");
	mDiffuseTextureNode->SetIndex(0);
	mDiffuseTextureNode->SetTexture(&mDiffuseTexture);

	mSpecularTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mSpecularTextureNode->SetId(7);
	mSpecularTextureNode->SetName("SpecularTexture");
	mSpecularTextureNode->SetIndex(1);
	mSpecularTextureNode->SetTexture(&mSpecularTexture);

	mDisplacementTextureNode = mSceneManager->CreateTextureNode(ShaderStage::VertexShader);
	mDisplacementTextureNode->SetId(8);
	mDisplacementTextureNode->SetName("DisplacementTexture");
	mDisplacementTextureNode->SetIndex(2);
	mDisplacementTextureNode->SetTexture(&mDisplacementTexture);

	mNormalTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mNormalTextureNode->SetId(9);
	mNormalTextureNode->SetName("NormalTexture");
	mNormalTextureNode->SetIndex(3);
	mNormalTextureNode->SetTexture(&mNormalTexture);

	mTransformNode = mSceneManager->CreateTransformNode();
	mTransformNode->SetId(10);
	mTransformNode->SetName("Transform");
	mTransformNode->GetTransform().SetPosition({ 0.0f, 0.0f, 0.0f });
	mTransformNode->GetTransform().SetScale({ 100.0f, 100.0f, 100.0f });

	mSceneManager->GetRoot().AddChild(mCameraNode);
	mCameraNode->AddChild(mShaderNode);
	mCameraNode->AddChild(mLightNode);
	mCameraNode->AddChild(mMaterialNode);
	mCameraNode->AddChild(mSamplerNode);
	mCameraNode->AddChild(mDiffuseTextureNode);
	mCameraNode->AddChild(mSpecularTextureNode);
	mCameraNode->AddChild(mDisplacementTextureNode);
	mCameraNode->AddChild(mNormalTextureNode);
	mCameraNode->AddChild(mTransformNode);
	mCameraNode->AddChild(mMeshNode);
}

void GameApp::OnTerminate()
{
	mCameraNode->Terminate();
	mLightNode->Terminate();
	mMaterialNode->Terminate();
	mSamplerNode->Terminate();
	mTransformNode->Terminate();

	mSceneManager->Purge();

	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mDiffuseTexture.Terminate();
	mSpecularTexture.Terminate();
	mDisplacementTexture.Terminate();
	mNormalTexture.Terminate();

	mSphereMesh.Destroy();
	mSkySphere.Destroy();

	mSphereMeshBuffer.Terminate();
	mSkySphereMeshBuffer.Terminate();
	Graphics::GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();
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

	auto inputSystem = Input::InputSystem::Get();

	inputSystem->Update();

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

	mSceneManager->Update(deltaTime);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();
	mSceneManager->Render();

	// Bind the input layout, vertex shader, and pixel shader
	//mVertexShader.Bind();
	//mPixelShader.Bind();

	////THIS IS SKYSPHERE

	//Maths::Matrix44 skySphereWorldMatrix = Maths::Matrix44::Scaling(50000.0f) * Maths::Matrix44::Translation(mCamera.GetPosition().x, mCamera.GetPosition().y, mCamera.GetPosition().z);
	//Maths::Matrix44 wvpSkySphere = skySphereWorldMatrix * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();

	//mDiffuseTexture.BindPS(0);
	//mDiffuseTexture.BindVS(0);

	//mSpecularTexture.BindPS(1);

	//mDisplacementTexture.BindVS(2);


	//	
	//planetTranslation = { 2041.0f, 0.0f, 0.0f };
	//planetScaling = { 100.0f, 100.0f, 100.0f };

	//float hack = 5.0f;
	//float speed = 10.0f;
	//WOWGE::Maths::Matrix44 planetWorldMatrix =
	//Maths::Matrix44::Scaling(planetScaling.x * hack, planetScaling.y * hack, planetScaling.z * hack) *
	//Maths::Matrix44::Translation(planetTranslation.x, planetTranslation.y, planetTranslation.z);

	//mDiffuseTexture.BindPS(0);
	//mDiffuseTexture.BindVS(0);

	//mSpecularTexture.BindPS(1);
	//mDisplacementTexture.BindVS(2);

	//TransformData planetsData;
	//planetsData.world = Maths::Transpose(planetWorldMatrix);
	//planetsData.wvp = Maths::Transpose(planetWorldMatrix * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix());
	//planetsData.cameraPosition = { mCamera.GetPosition().x, mCamera.GetPosition().y, mCamera.GetPosition().z, 0.0f };
	//planetsData.lightDirection = { mCamera.GetDirection().x, mCamera.GetDirection().y, mCamera.GetDirection().z, 0.0f };
	//planetsData.lightAmbient = mLight.ambient;
	//planetsData.lightDiffuse = mLight.diffuse;
	//planetsData.lightSpecular = mLight.specular;
	//planetsData.materialAmbient = mMaterial.ambient;
	//planetsData.materialDiffuse = mMaterial.diffuse;
	//planetsData.materialSpecular = mMaterial.specular;
	//planetsData.materialEmissive = mMaterial.emissive;
	//planetsData.materialPower = mMaterial.power;
	//planetsData.displacementFactor = 0.1f;

	//mConstantBuffer.Set(planetsData);
	//mConstantBuffer.BindVS();
	//mConstantBuffer.BindPS();

	//mSphereMeshBuffer.Render();

	graphicsSystem->EndRender();

}