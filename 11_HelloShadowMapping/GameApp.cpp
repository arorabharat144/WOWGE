#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

using namespace WOWGE;

namespace
{
	float speed = 50.0f;
	float rotationSpeed = 10.0f;

	uint32_t id = 0;

	Maths::Vector4 lightDirection{ 1.0f, -1.0f, 1.0f, 1.0f };
}

GameApp::GameApp() :
	mCameraNode(nullptr),
	mLightNode(nullptr),
	mPlaneMaterialNode(nullptr),
	mPlaneMeshNode(nullptr),
	mDebugQuadMeshNode(nullptr),
	mModelNode(nullptr),
	mRasterizerStateNode(nullptr),
	mDepthMapNode(nullptr),
	mDepthMapNode2ndPass(nullptr),
	mDebugDepthMapNode(nullptr),
	mSamplerNode(nullptr),
	mUISamplerNode(nullptr),
	mDepthSamplerNode(nullptr),
	mSceneManager(nullptr),
	mDepthMapShaderNode(nullptr),
	mShadowMapShaderNode(nullptr),
	mUIShaderNode(nullptr),
	mPlaneTextureNode(nullptr),
	mPlaneTransformNode(nullptr),
	mGroundShaderNode(nullptr),
	mGroundSamplerNode(nullptr)
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

	const float widthLightCamera = w * 0.1f;
	const float heightLightCamera = h * 0.1f;

	mWindow.Initialize(GetInstance(), GetAppName(), w, h);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	const float uiQuadWidth = 0.35f;
	const float uiQuadHeight = uiQuadWidth*graphicsSystem->GetAspectRatio();
	const Graphics::VertexPT uiVertices[] =
	{
		{ { -1.0f , -1.0f, 0.0f },{ 0.0f, 1.0f } } ,
		{ { -1.0f , -1.0f + uiQuadHeight, 0.0f },{ 0.0f, 0.0f } },
		{ { -1.0f + uiQuadWidth , -1.0f + uiQuadHeight, 0.0f },{ 1.0f, 0.0f } },
		{ { -1.0f + uiQuadWidth , -1.0f, 0.0f },{ 1.0f, 1.0f } }
	};

	const uint32_t indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	mDebugQuadMeshBuffer.Initialize(uiVertices, sizeof(Graphics::VertexPT), std::size(uiVertices), indices, std::size(indices));

	Graphics::MeshBuilder::CreatePlane(mPlane);
	mPlaneMeshBuffer.Initialize(mPlane);
	mPlaneTexture.Initialize("../Assets/Images/BrickSmallBrown.jpg");

	mRasterizerState.Initialize(Graphics::CullMode::None, Graphics::FillMode::Wireframe);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience
	mDepthVertexShader.Initialize(L"../Assets/Shaders/SkinnedDepthMap.fx", "VS", "vs_5_0", Graphics::BoneVertex::Format);
	mShadowMappingVertexShader.Initialize(L"../Assets/Shaders/StandardSkinning.fx", "VS", "vs_5_0", Graphics::BoneVertex::Format);
	mUIVertexShader.Initialize(L"../Assets/Shaders/SkinnedDepthMap.fx", "VSDebug", "vs_5_0", Graphics::VertexPT::Format);
	mGroundVertexShader.Initialize(L"../Assets/Shaders/Primitive.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	///////////////////////////////////////
	// Compile and create the pixel shader

	mGroundPixelShader.Initialize(L"../Assets/Shaders/Primitive.fx");
	mShadowMappingPixelShader.Initialize(L"../Assets/Shaders/StandardSkinning.fx");
	mUIPixelShader.Initialize(L"../Assets/Shaders/SkinnedDepthMap.fx", "PSDebug", "ps_5_0");

	mCamera.Initialize(0.785f, (float)w, (float)h, 1.0f, 5000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	//mCamera.Yaw(-30.0f);
	mCamera.SetPosition({ 0.0f, 10.0f,-100.0f });
	mTimer.Initialize();

	mLightCamera.Initialize(0.785f, (float)w, (float)h, 25.0f, 200.0f);
	//mLightCamera.InitializeOrthographicProjectionMatrix((float)w * 0.2f, (float)h * 0.2f);
	mLightCamera.InitializeOrthographicProjectionMatrix(widthLightCamera, heightLightCamera);
	mLightCamera.Pitch(30.0f);
	mLightCamera.SetPosition({ 0.0f, 50.0f,-40.0f });

	mCameraNode = mSceneManager->CreateCameraNode();
	mCameraNode->SetName("Camera");
	mCameraNode->SetCamera(&mCamera, 0);
	mCameraNode->SetCamera(&mLightCamera, 1);

	mLightNode = mSceneManager->CreateLightNode();
	mLightNode->SetName("Light");
	mLightNode->GetLight().ambient = Maths::Vector4::Black();
	mLightNode->GetLight().diffuse = Maths::Vector4::White();
	mLightNode->GetLight().direction = { -1.0f, -1.0f, 1.0f, 0.0f };
	mLightNode->GetLight().specular = Maths::Vector4::White();

	//mLightCamera.SetPosition(Maths::Vector3{ -1.0f, -1.0f, 1.0f } *-1.0f * 50.0f);

	mSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSamplerNode->SetName("Sampler");

	mGroundSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mGroundSamplerNode->SetName("GroundSampler");

	mUISamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Linear, Graphics::Sampler::AddressMode::Wrap);
	mUISamplerNode->SetName("UISampler");

	mDepthSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp, 1);
	mDepthSamplerNode->SetName("DepthSampler");

	mPlaneTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mPlaneTextureNode->SetName("PlaneTexture");
	mPlaneTextureNode->SetTexture(&mPlaneTexture);

	mRasterizerStateNode = mSceneManager->CreateRasterizerNode();
	mRasterizerStateNode->SetName("RasterizerState");
	mRasterizerStateNode->SetRasterizerStateNode(&mRasterizerState);
	mRasterizerStateNode->SetEnabled(false);

	const uint32_t depthMapSize = 4096;
	mDepthMap.Initialize(depthMapSize, depthMapSize);

	mRenderTarget = new Graphics::RenderTarget();
	mRenderTarget->Initialize(w, h, Graphics::ColorFormat::RGBA_U32);

	mDepthMapNode = mSceneManager->CreateDepthMapNode(1);
	mDepthMapNode->SetName("DepthMap");
	mDepthMapNode->SetDepthMap(&mDepthMap);
	//mDepthMapNode->SetHasTexture(true); 

	mDepthMapNode2ndPass = mSceneManager->CreateDepthMapNode(4);
	mDepthMapNode2ndPass->SetName("DepthMap2ndPass");
	mDepthMapNode2ndPass->SetDepthMap(&mDepthMap);
	mDepthMapNode2ndPass->SetHasTexture(true);

	mDebugDepthMapNode = mSceneManager->CreateDepthMapNode(0);
	mDebugDepthMapNode->SetName("DebugDepthMap");
	mDebugDepthMapNode->SetDepthMap(&mDepthMap);
	mDebugDepthMapNode->SetHasTexture(true);

	mDepthMapShaderNode = mSceneManager->CreateShaderNode();
	mDepthMapShaderNode->SetName("DepthMapShader");
	mDepthMapShaderNode->SetPixelShader(&mDepthPixelShader);
	mDepthMapShaderNode->SetVertexShader(&mDepthVertexShader);

	mShadowMapShaderNode = mSceneManager->CreateShaderNode();
	mShadowMapShaderNode->SetName("ShadowMapShader");
	mShadowMapShaderNode->SetPixelShader(&mShadowMappingPixelShader);
	mShadowMapShaderNode->SetVertexShader(&mShadowMappingVertexShader);

	mUIShaderNode = mSceneManager->CreateShaderNode();
	mUIShaderNode->SetName("UIShader");
	mUIShaderNode->SetPixelShader(&mUIPixelShader);
	mUIShaderNode->SetVertexShader(&mUIVertexShader);

	mGroundShaderNode = mSceneManager->CreateShaderNode();
	mGroundShaderNode->SetName("GroundShader");
	mGroundShaderNode->SetVertexShader(&mGroundVertexShader);
	mGroundShaderNode->SetPixelShader(&mGroundPixelShader);

	mPlaneTransformNode = mSceneManager->CreateTransformNode();
	mPlaneTransformNode->SetName("Transform");
	mPlaneTransformNode->GetTransform().SetPosition({ -200.0f, 0.0f, -150.0f });
	mPlaneTransformNode->GetTransform().SetScale({ 500.0f, 500.0f, 1.0f });
	mPlaneTransformNode->GetTransform().SetRotation({ 90.0f, 0.0f, 0.0f });

	mPlaneMeshNode = mSceneManager->CreateMeshNode();
	mPlaneMeshNode->SetMesh(&mPlaneMeshBuffer);

	mDebugQuadMeshNode = mSceneManager->CreateMeshNode();
	mDebugQuadMeshNode->SetMesh(&mDebugQuadMeshBuffer);

	mPlaneMaterialNode = mSceneManager->CreateMaterialNode();
	mPlaneMaterialNode->GetMaterial().ambient = Maths::Vector4::Black();
	mPlaneMaterialNode->GetMaterial().diffuse = Maths::Vector4::White();
	mPlaneMaterialNode->GetMaterial().specular = Maths::Vector4::Gray();
	mPlaneMaterialNode->GetMaterial().emissive = Maths::Vector4::Black();
	mPlaneMaterialNode->GetMaterial().power = 1.0f;

	mModelNode = mSceneManager->CreateModelNodeWithBones("Dancing Twerk.wow");
	mModelNode->SetName("MalcolmModel");
	mModelNode->GetTransformNode().GetTransform().SetPosition({ 0.0f, 0.0f, 10.0f });
	mModelNode->GetTransformNode().GetTransform().SetScale({ 0.2f, 0.2f, 0.2f });
	mMalcolmAnimationController.Initialize(mModelNode->GetModel());
	mMalcolmAnimationController.LoadAnimation("Twerk.wowanim");

	mSceneManager->GetRoot().AddChild(mCameraNode);

	mCameraNode->AddChild(mDepthMapNode);
	mDepthMapNode->AddChild(mDepthMapShaderNode);
	mDepthMapShaderNode->AddChild(mModelNode);

	mCameraNode->AddChild(mDepthMapNode2ndPass);
	mDepthMapNode2ndPass->AddChild(mRasterizerStateNode);
	mRasterizerStateNode->AddChild(mShadowMapShaderNode);
	mShadowMapShaderNode->AddChild(mLightNode);
	mShadowMapShaderNode->AddChild(mDepthSamplerNode);
	mShadowMapShaderNode->AddChild(mSamplerNode);
	
	mDepthMapNode2ndPass->AddChild(mGroundShaderNode);
	mGroundShaderNode->AddChild(mGroundSamplerNode);
	mGroundSamplerNode->AddChild(mPlaneTransformNode);
	mPlaneTransformNode->AddChild(mPlaneTextureNode);
	mPlaneTextureNode->AddChild(mPlaneMaterialNode);
	mPlaneMaterialNode->AddChild(mPlaneMeshNode);
	//mSamplerNode->AddChild(mPlaneTransformNode);
	//mPlaneTransformNode->AddChild(mPlaneTextureNode);
	//mPlaneTextureNode->AddChild(mPlaneMaterialNode);
	//mPlaneMaterialNode->AddChild(mPlaneMeshNode);

	mSamplerNode->AddChild(mModelNode);

	mDepthMapNode2ndPass->AddChild(mUIShaderNode);
	mUIShaderNode->AddChild(mUISamplerNode);
	mUISamplerNode->AddChild(mDebugDepthMapNode);
	mDebugDepthMapNode->AddChild(mDebugQuadMeshNode);
}

void GameApp::OnTerminate()
{
	mCameraNode->Terminate();
	mLightNode->Terminate();

	mSamplerNode->Terminate();
	mDepthMapNode->Terminate();

	mSceneManager->Purge();

	mDepthVertexShader.Terminate();

	mShadowMappingVertexShader.Terminate();
	mShadowMappingPixelShader.Terminate();

	mUIVertexShader.Terminate();
	mUIPixelShader.Terminate();

	mGroundVertexShader.Terminate();
	mGroundPixelShader.Terminate();

	mPlaneTexture.Terminate();
	mPlane.Destroy();
	mPlaneMeshBuffer.Terminate();
	mDebugQuadMeshBuffer.Terminate();

	mRenderTarget->Terminate();
	mDepthMap.Terminate();
	SafeDelete(mRenderTarget);

	mRasterizerState.Terminate();

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
	mLightCamera.UpdateViewMatrix();

	mMalcolmAnimationController.Play("Twerk", deltaTime, true);
	mMalcolmAnimationController.Update();

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