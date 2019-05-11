#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

using namespace WOWGE;

namespace
{
	float speed = 50.0f;
	float rotationSpeed = 10.0f;

	uint32_t id = 0;
}

GameApp::GameApp():
	mCameraNode(nullptr),
	mLightNode(nullptr),
	mPlaneMeshNode(nullptr),
	mSkySphereMeshNode(nullptr),
	mRasterizerStateNode(nullptr),
	mRenderTargetNode(nullptr),
	mRenderTargetNode2ndPass(nullptr),
	mSamplerNode(nullptr),
	mSceneManager(nullptr),
	mShaderNodeSkySphere(nullptr),
	mShaderNode(nullptr),
	mShaderNodePostProcessing(nullptr),
	mTextureNode(nullptr),
	mModelNode(nullptr),
	mSkySphereSamplerNode(nullptr)
{
	mSceneManager = new WOWGE::Graphics::SceneManager(mMatrixStack);
}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	const uint32_t w = 1920;
	const uint32_t h = 1080;

	mWindow.Initialize(GetInstance(), GetAppName(), w, h);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	Graphics::MeshBuilder::CreateSkySphere(mSkySphere, 100, 100);
	mSkySphereMeshBuffer.Initialize(mSkySphere);
	mSkyTexture.Initialize("../Assets/Images/Stars.jpg");

	Graphics::MeshBuilder::CreatePostProcessingPlane(mPlane);
	mPlaneMeshBuffer.Initialize(mPlane);

	mRasterizerState.Initialize(Graphics::CullMode::None, Graphics::FillMode::Wireframe);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	mSkySphereVertexShader.Initialize(L"../Assets/Shaders/SkySphere.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mVertexShader.Initialize(L"../Assets/Shaders/NonShadowSkinning.fx", "VS", "vs_5_0", Graphics::BoneVertex::Format);
	mVertexShaderPostProcessing.Initialize(L"../Assets/Shaders/SkinnedPostProcessing.fx", "VS", "vs_5_0", Graphics::BoneVertex::Format);

	///////////////////////////////////////
	// Compile and create the pixel shader

	mSkySpherePixelShader.Initialize(L"../Assets/Shaders/SkySphere.fx");
	mPixelShader.Initialize(L"../Assets/Shaders/NonShadowSkinning.fx");
	mPixelShaderPostProcessing.Initialize(L"../Assets/Shaders/SkinnedPostProcessing.fx");

	mCamera.Initialize(0.785f, (float)w, (float)h, 1.0f, 5000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.SetPosition({ 0.0f, 0.0f,-100.0f });
	mTimer.Initialize();

	mCameraNode = mSceneManager->CreateCameraNode();
	mCameraNode->SetName("Camera");
	mCameraNode->SetCamera(&mCamera);
	mCameraNode->SetCamera(&mCamera, 1);
	 
	mLightNode = mSceneManager->CreateLightNode();
	mLightNode->SetName("Light");
	mLightNode->GetLight().ambient = Maths::Vector4::White();
	mLightNode->GetLight().diffuse = Maths::Vector4::White();
	mLightNode->GetLight().direction = { -1.0f, -1.0f, 1.0f, 0.0f };
	mLightNode->GetLight().specular = Maths::Vector4::White();

	mSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSamplerNode->SetName("Sampler");

	mSkySphereSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSkySphereSamplerNode->SetName("SkySphereSampler");
	
	mTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mTextureNode->SetName("SkySphereTexture");
	mTextureNode->SetTexture(&mSkyTexture);

	mRasterizerStateNode = mSceneManager->CreateRasterizerNode();
	mRasterizerStateNode->SetName("RasterizerState");
	mRasterizerStateNode->SetRasterizerStateNode(&mRasterizerState);
	mRasterizerStateNode->SetEnabled(false);

	mRenderTarget = new Graphics::RenderTarget();
	mRenderTarget->Initialize(w, h, Graphics::ColorFormat::RGBA_U32);

	mRenderTargetNode = mSceneManager->CreateRenderTargetNode(0);
	mRenderTargetNode->SetName("RenderTarget");
	mRenderTargetNode->SetRenderTarget(mRenderTarget);

	mRenderTargetNode2ndPass = mSceneManager->CreateRenderTargetNode(0);
	mRenderTargetNode2ndPass->SetName("RenderTargetPostProcessing");
	mRenderTargetNode2ndPass->SetRenderTarget(mRenderTarget);
	mRenderTargetNode2ndPass->SetHasTexture(true);

	mShaderNodeSkySphere = mSceneManager->CreateShaderNode();
	//mShaderNodeSkySphere->SetName("ShaderSkySphere");
	mShaderNodeSkySphere->SetVertexShader(&mSkySphereVertexShader);
	mShaderNodeSkySphere->SetPixelShader(&mSkySpherePixelShader);

	mShaderNode = mSceneManager->CreateShaderNode();
	mShaderNode->SetName("Shader");
	mShaderNode->SetPixelShader(&mPixelShader);
	mShaderNode->SetVertexShader(&mVertexShader);

	mShaderNodePostProcessing = mSceneManager->CreateShaderNode();
	mShaderNodePostProcessing->SetName("ShaderPostProcessing");
	mShaderNodePostProcessing->SetPixelShader(&mPixelShaderPostProcessing);
	mShaderNodePostProcessing->SetVertexShader(&mVertexShaderPostProcessing);

	mSkySphereTransformNode = mSceneManager->CreateTransformNode();
	mSkySphereTransformNode->SetName("TransformSkySphere");
	mSkySphereTransformNode->GetTransform().SetPosition({ 0.0f, 0.0f, 0.0f });
	mSkySphereTransformNode->GetTransform().SetScale({ 1000.0f, 1000.0f, 1000.0f });

	mModelNode = mSceneManager->CreateModelNodeWithBones("Liam.wow");
	mModelNode->SetName("LiamModel");
	mLiamAnimationController.Initialize(mModelNode->GetModel());
	mLiamAnimationController.LoadAnimation("LiamIdle.wowanim");

	mPlaneMeshNode = mSceneManager->CreateMeshNode();
	mPlaneMeshNode->SetMesh(&mPlaneMeshBuffer);

	mSkySphereMeshNode = mSceneManager->CreateMeshNode();
	mSkySphereMeshNode->SetMesh(&mSkySphereMeshBuffer);

	mSceneManager->GetRoot().AddChild(mCameraNode);
	mCameraNode->AddChild(mRenderTargetNode);
	mRenderTargetNode->AddChild(mRasterizerStateNode);
	mRasterizerStateNode->AddChild(mShaderNode);
	mShaderNode->AddChild(mLightNode);
	mShaderNode->AddChild(mSamplerNode);
	mSamplerNode->AddChild(mModelNode);

	mRenderTargetNode->AddChild(mShaderNodeSkySphere);
	mShaderNodeSkySphere->AddChild(mSkySphereSamplerNode);
	mSkySphereSamplerNode->AddChild(mSkySphereTransformNode);
	mSkySphereTransformNode->AddChild(mTextureNode);
	mTextureNode->AddChild(mSkySphereMeshNode);

	mCameraNode->AddChild(mRenderTargetNode2ndPass);
	mRenderTargetNode2ndPass->AddChild(mShaderNodePostProcessing);
	mShaderNodePostProcessing->AddChild(mPlaneMeshNode);
}

void GameApp::OnTerminate()
{
	mCameraNode->Terminate();
	mLightNode->Terminate();
	
	mSamplerNode->Terminate();

	mSceneManager->Purge();

	mSkySphereVertexShader.Terminate();
	mSkySpherePixelShader.Terminate();

	mVertexShaderPostProcessing.Terminate();
	mPixelShaderPostProcessing.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mSkyTexture.Terminate();
	mSkySphere.Destroy();
	mPlane.Destroy();
	mPlaneMeshBuffer.Terminate();

	mRenderTarget->Terminate();
	SafeDelete(mRenderTarget);

	mRasterizerState.Terminate();

	mSkySphereMeshBuffer.Terminate();
	Gui::Terminate();
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

	if (inputSystem->IsKeyPressed(Input::KeyCode::F1))
	{
		mRasterizerStateNode->SetEnabled(!mRasterizerStateNode->GetEnabled());
	}

	mCamera.UpdateViewMatrix();

	mLiamAnimationController.Play("LiamIdle", deltaTime, true);
	mLiamAnimationController.Update();

	// Draw mesh

	mSceneManager->Update(deltaTime);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();
	mSceneManager->Render();

	//Gui::BeginRender(deltaTime);

	//ImGui::SetNextWindowSize({ 500.0f, 500.0f });
	//ImGui::Begin("Test");

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.30f);
	//ImGui::SliderInt("int test", &a, 0, 10); ImGui::SameLine();
	//ImGui::SliderAngle("int test 2", &guiAngle);

	//ImGui::ColorPicker4("ColorPicker", guiColor);
	//
	//ImGui::End();



	//Gui::EndRender();


	graphicsSystem->EndRender();

}