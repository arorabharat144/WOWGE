#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

using namespace WOWGE;

namespace
{
	float speed = 50.0f;
	float rotationSpeed = 10.0f;
	float random;

	Maths::Vector4 staticEffectShaderValues;
	float shaderMode = 0;
}

GameApp::GameApp() :
	mRenderTarget(nullptr),
	mCameraNode(nullptr),
	mConstantBufferNode1(nullptr),
	mConstantBufferNode2(nullptr),
	mLightNode(nullptr),
	mPlaneMaterialNode(nullptr),
	mSkySphereMeshNode(nullptr),
	mPlaneMeshNode(nullptr),
	mPostProcessingPlaneMeshNode(nullptr),
	mDebugQuadMeshNode(nullptr),
	mModelNode1(nullptr),
	mModelNode2(nullptr),
	mModelNode3(nullptr),
	mModelNode4(nullptr),
	mModelNode5(nullptr),
	mRasterizerStateNode(nullptr),
	mRenderTargetNode1(nullptr),
	mRenderTargetNode2(nullptr),
	mDepthMapNode(nullptr),
	mDepthMapNode2ndPass(nullptr),
	mDebugDepthMapNode(nullptr),
	mSkySphereSamplerNode(nullptr),
	mSamplerNode(nullptr),
	mUISamplerNode(nullptr),
	mDepthSamplerNode(nullptr),
	mSceneManager(nullptr),
	mSkySphereShaderNode(nullptr),
	mDepthMapShaderNode(nullptr),
	mShadowMapShaderNode(nullptr),
	mTVStaticShaderNode(nullptr),
	mUIShaderNode(nullptr),
	mSkySphereTextureNode(nullptr),
	mPlaneTextureNode(nullptr),
	mSkySphereTransformNode(nullptr),
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

	srand(static_cast<unsigned int>(time(static_cast<time_t>(0))));

	const float widthLightCamera = w * 0.1f;
	const float heightLightCamera = h * 0.1f;

	mWindow.Initialize(GetInstance(), GetAppName(), w, h);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	Graphics::MeshBuilder::CreateSkySphere(mSkySphere, 200, 200);
	mSkySphereMeshBuffer.Initialize(mSkySphere);
	mSkySphereTexture.Initialize("../Assets/Images/Woods.jpg");

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

	mDebugQuadMeshBuffer.Initialize(uiVertices, sizeof(Graphics::VertexPT), static_cast<uint32_t>(std::size(uiVertices)), indices, static_cast<uint32_t>(std::size(indices)));

	Graphics::MeshBuilder::CreatePlane(mPlane);
	mPlaneMeshBuffer.Initialize(mPlane);
	mPlaneTexture.Initialize("../Assets/Images/Ground.jpg");

	Graphics::MeshBuilder::CreatePostProcessingPlane(mPostProcessingPlane);
	mPostProcessingPlaneMeshBuffer.Initialize(mPostProcessingPlane);

	mRasterizerState.Initialize(Graphics::CullMode::None, Graphics::FillMode::Wireframe);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience
	mSkySphereVertexShader.Initialize(L"../Assets/Shaders/SkySphere.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mDepthVertexShader.Initialize(L"../Assets/Shaders/SkinnedDepthMap.fx", "VS", "vs_5_0", Graphics::BoneVertex::Format);
	mShadowMappingVertexShader.Initialize(L"../Assets/Shaders/StandardSkinning.fx", "VS", "vs_5_0", Graphics::BoneVertex::Format);
	mGroundVertexShader.Initialize(L"../Assets/Shaders/Primitive.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mTVStaticVertexShader.Initialize(L"../Assets/Shaders/StaticEffect.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mUIVertexShader.Initialize(L"../Assets/Shaders/SkinnedDepthMap.fx", "VSDebug", "vs_5_0", Graphics::VertexPT::Format);
	///////////////////////////////////////
	// Compile and create the pixel shader

	mSkySpherePixelShader.Initialize(L"../Assets/Shaders/SkySphere.fx");
	mShadowMappingPixelShader.Initialize(L"../Assets/Shaders/StandardSkinning.fx");
	mGroundPixelShader.Initialize(L"../Assets/Shaders/Primitive.fx");

	mTVStaticPixelShader.Initialize(L"../Assets/Shaders/StaticEffect.fx");
	mUIPixelShader.Initialize(L"../Assets/Shaders/SkinnedDepthMap.fx", "PSDebug", "ps_5_0");

	mCamera.Initialize(0.785f, (float)w, (float)h, 1.0f, 5000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.SetPosition({ 0.0f, 10.0f,-100.0f });
	mTimer.Initialize();

	mLightCamera.Initialize(0.785f, (float)w, (float)h, 25.0f, 500.0f);
	mLightCamera.InitializeOrthographicProjectionMatrix(widthLightCamera, heightLightCamera);
	mLightCamera.Pitch(30.0f);
	mLightCamera.SetPosition({ 0.0f, 50.0f, -40.0f });

	mRenderTarget = new Graphics::RenderTarget();
	mRenderTarget->Initialize(w, h, Graphics::ColorFormat::RGBA_U32);

	mCameraNode = mSceneManager->CreateCameraNode();
	mCameraNode->SetName("Camera");
	mCameraNode->SetCamera(&mCamera, 0);
	mCameraNode->SetCamera(&mLightCamera, 1);

	mLightNode = mSceneManager->CreateLightNode();
	mLightNode->SetName("Light");
	mLightNode->GetLight().ambient = Maths::Vector4::Gray();
	mLightNode->GetLight().diffuse = Maths::Vector4::White();
	mLightNode->GetLight().direction = { -1.0f, -1.0f, 1.0f, 0.0f };
	mLightNode->GetLight().specular = Maths::Vector4::White();

	mSkySphereSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSkySphereSamplerNode->SetName("SkySphereSampler");

	mSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSamplerNode->SetName("Sampler");

	mGroundSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mGroundSamplerNode->SetName("GroundSampler");

	mUISamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Linear, Graphics::Sampler::AddressMode::Wrap);
	mUISamplerNode->SetName("UISampler");

	mDepthSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp, 1);
	mDepthSamplerNode->SetName("DepthSampler");

	mSkySphereTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mSkySphereTextureNode->SetName("SkySphereTexture");
	mSkySphereTextureNode->SetTexture(&mSkySphereTexture);

	mPlaneTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mPlaneTextureNode->SetName("PlaneTexture");
	mPlaneTextureNode->SetTexture(&mPlaneTexture);

	mRasterizerStateNode = mSceneManager->CreateRasterizerNode();
	mRasterizerStateNode->SetName("RasterizerState");
	mRasterizerStateNode->SetRasterizerStateNode(&mRasterizerState);
	mRasterizerStateNode->SetEnabled(false);

	const uint32_t depthMapSize = 16384;
	mDepthMap.Initialize(depthMapSize, depthMapSize);

	mRenderTarget = new Graphics::RenderTarget();
	mRenderTarget->Initialize(w, h, Graphics::ColorFormat::RGBA_U32);

	mRenderTargetNode1 = mSceneManager->CreateRenderTargetNode(0);
	mRenderTargetNode1->SetName("RenderTarget1");
	mRenderTargetNode1->SetRenderTarget(mRenderTarget);

	mRenderTargetNode2 = mSceneManager->CreateRenderTargetNode(0);
	mRenderTargetNode2->SetName("RenderTarget2");
	mRenderTargetNode2->SetRenderTarget(mRenderTarget);
	mRenderTargetNode2->SetHasTexture(true);

	mDepthMapNode = mSceneManager->CreateDepthMapNode(1);
	mDepthMapNode->SetName("DepthMap");
	mDepthMapNode->SetDepthMap(&mDepthMap);

	mDepthMapNode2ndPass = mSceneManager->CreateDepthMapNode(4);
	mDepthMapNode2ndPass->SetName("DepthMap2ndPass");
	mDepthMapNode2ndPass->SetDepthMap(&mDepthMap);
	mDepthMapNode2ndPass->SetHasTexture(true);

	mDebugDepthMapNode = mSceneManager->CreateDepthMapNode(0);
	mDebugDepthMapNode->SetName("DebugDepthMap");
	mDebugDepthMapNode->SetDepthMap(&mDepthMap);
	mDebugDepthMapNode->SetHasTexture(true);

	mSkySphereShaderNode = mSceneManager->CreateShaderNode();
	mSkySphereShaderNode->SetName("SkySphereShader");
	mSkySphereShaderNode->SetPixelShader(&mSkySpherePixelShader);
	mSkySphereShaderNode->SetVertexShader(&mSkySphereVertexShader);

	mDepthMapShaderNode = mSceneManager->CreateShaderNode();
	mDepthMapShaderNode->SetName("DepthMapShader");
	mDepthMapShaderNode->SetPixelShader(&mDepthPixelShader);
	mDepthMapShaderNode->SetVertexShader(&mDepthVertexShader);

	mShadowMapShaderNode = mSceneManager->CreateShaderNode();
	mShadowMapShaderNode->SetName("ShadowMapShader");
	mShadowMapShaderNode->SetPixelShader(&mShadowMappingPixelShader);
	mShadowMapShaderNode->SetVertexShader(&mShadowMappingVertexShader);

	mGroundShaderNode = mSceneManager->CreateShaderNode();
	mGroundShaderNode->SetName("GroundShader");
	mGroundShaderNode->SetPixelShader(&mGroundPixelShader);
	mGroundShaderNode->SetVertexShader(&mGroundVertexShader);

	mTVStaticShaderNode = mSceneManager->CreateShaderNode();
	mTVStaticShaderNode->SetName("NoiseShader");
	mTVStaticShaderNode->SetPixelShader(&mTVStaticPixelShader);
	mTVStaticShaderNode->SetVertexShader(&mTVStaticVertexShader);

	random = Maths::RandomFloat(0.0f, RAND_MAX);
	mConstantBufferNode1 = mSceneManager->CreateConstantBufferNode<ConstantBuffer>(0);
	staticEffectShaderValues = { mTimer.GetElapsedTime(), 0.009f, random, 1.0f };
	mConstantBufferNode1->GetData().values = staticEffectShaderValues;

	mConstantBufferNode2 = mSceneManager->CreateConstantBufferNode<ConstantBuffer>(1);

	mUIShaderNode = mSceneManager->CreateShaderNode();
	mUIShaderNode->SetName("UIShader");
	mUIShaderNode->SetPixelShader(&mUIPixelShader);
	mUIShaderNode->SetVertexShader(&mUIVertexShader);

	mSkySphereTransformNode = mSceneManager->CreateTransformNode();
	mSkySphereTransformNode->SetName("SkySphereTransform");
	mSkySphereTransformNode->GetTransform().SetScale({ 4000.0f, 4000.0f, 4000.0f });

	mPlaneTransformNode = mSceneManager->CreateTransformNode();
	mPlaneTransformNode->SetName("PlaneTransform");
	mPlaneTransformNode->GetTransform().SetPosition({ -200.0f, 0.0f, -150.0f });
	mPlaneTransformNode->GetTransform().SetScale({ 500.0f, 500.0f, 1.0f });
	mPlaneTransformNode->GetTransform().SetRotation({ 90.0f, 0.0f, 0.0f });

	mSkySphereMeshNode = mSceneManager->CreateMeshNode();
	mSkySphereMeshNode->SetName("SkySphereMesh");
	mSkySphereMeshNode->SetMesh(&mSkySphereMeshBuffer);

	mPlaneMeshNode = mSceneManager->CreateMeshNode();
	mPlaneMeshNode->SetName("PlaneMesh");
	mPlaneMeshNode->SetMesh(&mPlaneMeshBuffer);

	mPostProcessingPlaneMeshNode = mSceneManager->CreateMeshNode();
	mPostProcessingPlaneMeshNode->SetName("PostProcessingPlaneMesh");
	mPostProcessingPlaneMeshNode->SetMesh(&mPostProcessingPlaneMeshBuffer);

	mDebugQuadMeshNode = mSceneManager->CreateMeshNode();
	mDebugQuadMeshNode->SetName("DebugQuadMesh");
	mDebugQuadMeshNode->SetMesh(&mDebugQuadMeshBuffer);

	mPlaneMaterialNode = mSceneManager->CreateMaterialNode();
	mPlaneMaterialNode->SetName("PlaneMaterial");
	mPlaneMaterialNode->GetMaterial().ambient = Maths::Vector4::White();
	mPlaneMaterialNode->GetMaterial().diffuse = Maths::Vector4::White();
	mPlaneMaterialNode->GetMaterial().specular = Maths::Vector4::Black();
	mPlaneMaterialNode->GetMaterial().emissive = Maths::Vector4::Black();
	mPlaneMaterialNode->GetMaterial().power = 1.0f;

	mModelNode1 = mSceneManager->CreateModelNodeWithBones("Maw.wow");
	mModelNode1->SetName("MawModel");
	mModelNode1->GetTransformNode().GetTransform().SetPosition({ 20.0f, 0.0f, -5.0f });
	mModelNode1->GetTransformNode().GetTransform().SetScale({ 0.2f, 0.2f, 0.2f });
	mMawAnimationController.Initialize(mModelNode1->GetModel());
	mMawAnimationController.LoadAnimation("MawIdle.wowanim");

	mModelNode2 = mSceneManager->CreateModelNodeWithBones("Mutant.wow");
	mModelNode2->SetName("MutantModel");
	mModelNode2->GetTransformNode().GetTransform().SetPosition({ -20.0f, 0.0f, 0.0f });
	mModelNode2->GetTransformNode().GetTransform().SetScale({ 0.2f, 0.2f, 0.2f });
	mMutantAnimationController.Initialize(mModelNode2->GetModel());
	mMutantAnimationController.LoadAnimation("MutantIdle.wowanim");

	mModelNode3 = mSceneManager->CreateModelNodeWithBones("SamsungTV.wow");
	mModelNode3->SetName("SamsungTVModel");
	mModelNode3->GetTransformNode().GetTransform().SetPosition({ 50.0f, 0.0f, 0.0f });
	mModelNode3->GetTransformNode().GetTransform().SetRotation({ 90.0f, 0.0f, 0.0f });
	mModelNode3->GetTransformNode().GetTransform().SetScale({ 2.0f, 2.0f, 2.0f });

	mModelNode4 = mSceneManager->CreateModelNodeWithBones("OldHouse.wow");
	mModelNode4->SetName("OldHouseModelNode");
	mModelNode4->GetTransformNode().GetTransform().SetPosition({ -40.0f, 0.0f, 45.0f });
	mModelNode4->GetTransformNode().GetTransform().SetRotation({ 0.0f, 180.0f, 0.0f });
	mModelNode4->GetTransformNode().GetTransform().SetScale({ 0.35f, 0.35f, 0.35f });

	mModelNode5 = mSceneManager->CreateModelNodeWithBones("Building_01.wow");
	mModelNode5->SetName("Building_01ModelNode");
	mModelNode5->GetTransformNode().GetTransform().SetPosition({ 50.0f, 0.0f, 30.0f });
	mModelNode5->GetTransformNode().GetTransform().SetScale({ 2.0f, 2.0f, 2.0f });

	//Connecting Nodes
	mSceneManager->GetRoot().AddChild(mCameraNode);

	mCameraNode->AddChild(mDepthMapNode);
	mDepthMapNode->AddChild(mDepthMapShaderNode);
	mDepthMapShaderNode->AddChild(mModelNode1);
	mDepthMapShaderNode->AddChild(mModelNode2);
	mDepthMapShaderNode->AddChild(mModelNode3);
	mDepthMapShaderNode->AddChild(mModelNode4);
	mDepthMapShaderNode->AddChild(mModelNode5);

	mCameraNode->AddChild(mRenderTargetNode1);
	mRenderTargetNode1->AddChild(mDepthMapNode2ndPass);
	mDepthMapNode2ndPass->AddChild(mRasterizerStateNode);
	mRasterizerStateNode->AddChild(mShadowMapShaderNode);
	mShadowMapShaderNode->AddChild(mLightNode);
	mShadowMapShaderNode->AddChild(mDepthSamplerNode);
	mShadowMapShaderNode->AddChild(mSamplerNode);

	//SkySphere Nodes
	mDepthMapNode2ndPass->AddChild(mSkySphereShaderNode);
	mSkySphereShaderNode->AddChild(mSkySphereSamplerNode);
	mSkySphereSamplerNode->AddChild(mSkySphereTransformNode);
	mSkySphereTransformNode->AddChild(mSkySphereTextureNode);
	mSkySphereTextureNode->AddChild(mSkySphereMeshNode);

	//Ground Nodes
	mDepthMapNode2ndPass->AddChild(mGroundShaderNode);
	mGroundShaderNode->AddChild(mGroundSamplerNode);
	mGroundSamplerNode->AddChild(mPlaneTransformNode);
	mPlaneTransformNode->AddChild(mPlaneTextureNode);
	mPlaneTextureNode->AddChild(mPlaneMaterialNode);
	mPlaneMaterialNode->AddChild(mPlaneMeshNode);
	mSamplerNode->AddChild(mModelNode1);
	mSamplerNode->AddChild(mModelNode2);
	mSamplerNode->AddChild(mModelNode3);
	mSamplerNode->AddChild(mModelNode4);
	mSamplerNode->AddChild(mModelNode5);

	mCameraNode->AddChild(mRenderTargetNode2);
	mRenderTargetNode2->AddChild(mConstantBufferNode1);
	mConstantBufferNode1->AddChild(mConstantBufferNode2);
	mConstantBufferNode2->AddChild(mTVStaticShaderNode);
	mTVStaticShaderNode->AddChild(mPostProcessingPlaneMeshNode);

	//Debug UI
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

	mSkySphereVertexShader.Terminate();
	mSkySpherePixelShader.Terminate();

	mDepthVertexShader.Terminate();

	mShadowMappingVertexShader.Terminate();
	mShadowMappingPixelShader.Terminate();

	mGroundVertexShader.Terminate();
	mGroundPixelShader.Terminate();

	mTVStaticVertexShader.Terminate();
	mTVStaticPixelShader.Terminate();

	mUIVertexShader.Terminate();
	mUIPixelShader.Terminate();
	mSkySphereTexture.Terminate();
	mPlaneTexture.Terminate();
	mSkySphere.Destroy();
	mPlane.Destroy();
	mPostProcessingPlane.Destroy();
	mSkySphereMeshBuffer.Terminate();
	mPlaneMeshBuffer.Terminate();
	mPostProcessingPlaneMeshBuffer.Terminate();
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

	if (inputSystem->IsKeyPressed(Input::KeyCode::ONE))
	{
		shaderMode = 0.0f;
	}
	if (inputSystem->IsKeyPressed(Input::KeyCode::TWO))
	{
		shaderMode = 1.0f;
	}
	if (inputSystem->IsKeyPressed(Input::KeyCode::THREE))
	{
		shaderMode = 2.0f;
	}
	if (inputSystem->IsKeyPressed(Input::KeyCode::FOUR))
	{
		shaderMode = 3.0f;
	}

	mSkySphereTransformNode->GetTransform().SetPosition(mCamera.GetPosition());

	mCamera.UpdateViewMatrix();
	mLightCamera.UpdateViewMatrix();

	staticEffectShaderValues.x = mTimer.GetElapsedTime();
	staticEffectShaderValues.z = random;
	mConstantBufferNode1->GetData().values = staticEffectShaderValues;
	mConstantBufferNode2->GetData().values = { shaderMode, 0.0f, 0.0f, 0.0f };

	mMawAnimationController.Play("MawIdle", deltaTime, true);
	mMawAnimationController.Update();

	mMutantAnimationController.Play("MutantIdle", deltaTime, true);
	mMutantAnimationController.Update();

	// Draw mesh

	mSceneManager->Update(deltaTime);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();
	mSceneManager->Render();

	Gui::BeginRender(deltaTime);

	ImGui::SetNextWindowSize({ 300.0f, 400.0f });
	ImGui::Begin("Shader Values Modifier");
	ImGui::SetWindowPos({ 1600.0f, 600.0f });

	ImGui::SliderFloat("Noise \nIntensity", &staticEffectShaderValues.y, 0.001f, 0.05f);
	ImGui::SliderFloat("Scanlines \nIntensity", &staticEffectShaderValues.w, 0.1f, 10.0f);

	ImGui::Text("Press F1 To Turn On/Off Wireframe mode\n");
	ImGui::Text("Press 1 to change for monochrome output\n");
	ImGui::Text("Press 2 to change for scary purple output\n");
	ImGui::Text("Press 3 to change for original output\n");
	ImGui::Text("Press 4 to change for sketchbook output\n");
	ImGui::End();

	Gui::EndRender();

	graphicsSystem->EndRender();
}