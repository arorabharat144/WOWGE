#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

using namespace WOWGE;

namespace
{
	float speed = 50.0f;
	float rotationSpeed = 10.0f;

	uint32_t danceSelector{ 0 };

	Maths::Vector4 lightDirection{ 1.0f, -1.0f, 1.0f, 1.0f };

	bool toggleBones{ false };
}

GameApp::GameApp() :
	mCameraNode(nullptr),
	mLightNode(nullptr),
	mPlaneMaterialNode(nullptr),
	mPlaneMeshNode(nullptr),
	mSkySphereMeshNode(nullptr),
	mWarrokModelNode(nullptr),
	mPirateModelNode(nullptr),
	mRasterizerStateNode(nullptr),
	mDepthMapNode(nullptr),
	mDepthMapNode2ndPass(nullptr),
	mSamplerNode(nullptr),
	mDepthSamplerNode(nullptr),
	mSkySphereSamplerNode(nullptr),
	mSceneManager(nullptr),
	mDepthMapShaderNode(nullptr),
	mShadowMapShaderNode(nullptr),
	mSkySphereShaderNode(nullptr),
	mPlaneTextureNode(nullptr),
	mSkySphereTextureNode(nullptr),
	mPlaneTransformNode(nullptr),
	mSkySphereTransformNode(nullptr)
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

	const float widthLightCamera = w*0.2f;
	const float heightLightCamera = h*0.2f;

	mWindow.Initialize(GetInstance(), GetAppName(), w, h);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	Graphics::SimpleDraw::Initialize();

	Graphics::MeshBuilder::CreateSkySphere(mSkySphere, 200, 200);
	mSkySphereMeshBuffer.Initialize(mSkySphere);
	mSkySphereTexture.Initialize("../../Assets/Images/WoodsCompressed.jpg");

	Graphics::SkinnedMeshBuilder::CreatePlane(mPlane);
	mPlaneMeshBuffer.Initialize(mPlane);
	mPlaneTexture.Initialize("../../Assets/Images/BrickSmallBrown.jpg");

	mRasterizerState.Initialize(Graphics::CullMode::None, Graphics::FillMode::Solid);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience
	mSkySphereVertexShader.Initialize(L"../../Assets/Shaders/SkySphere.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mDepthVertexShader.Initialize(L"../../Assets/Shaders/SkinnedDepthMap.fx", "VS", "vs_5_0", Graphics::BoneVertex::Format);
	mShadowMappingVertexShader.Initialize(L"../../Assets/Shaders/StandardSkinning.fx", "VS", "vs_5_0", Graphics::BoneVertex::Format);
	///////////////////////////////////////
	// Compile and create the pixel shader

	mSkySpherePixelShader.Initialize(L"../../Assets/Shaders/SkySphere.fx");
	mShadowMappingPixelShader.Initialize(L"../../Assets/Shaders/StandardSkinning.fx");

	mCamera.Initialize(0.785f, (float)w, (float)h, 1.0f, 5000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.Pitch(15.0f);
	mCamera.SetPosition({ 0.0f, 100.0f,-500.0f });
	mTimer.Initialize();

	mLightCamera.Initialize(0.785f, (float)w, (float)h, 25.0f, 500.0f);
	mLightCamera.InitializeOrthographicProjectionMatrix(widthLightCamera, heightLightCamera);
	mLightCamera.Pitch(30.0f);
	mLightCamera.SetPosition({ 0.0f, 50.0f,-100.0f });

	mCameraNode = mSceneManager->CreateCameraNode();
	mCameraNode->SetName("Camera");
	mCameraNode->SetCamera(&mCamera, 0);
	mCameraNode->SetCamera(&mLightCamera, 1);

	mLightNode = mSceneManager->CreateLightNode();
	mLightNode->SetName("Light");
	mLightNode->GetLight().ambient = Maths::Vector4::Black();
	mLightNode->GetLight().diffuse = Maths::Vector4::White();
	mLightNode->GetLight().direction = { -1.0f, -1.0f, 1.0f, 1.0f };
	mLightNode->GetLight().specular = { 0.5f, 0.5f, 0.5f, 1.0f };

	mSkySphereSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSkySphereSamplerNode->SetName("SkySphereSampler");

	mSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap, 0);
	mSamplerNode->SetName("Sampler");

	mDepthSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp, 1);
	mDepthSamplerNode->SetName("DepthSampler");

	mSkySphereTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mSkySphereTextureNode->SetName("SkySphereTexture");
	mSkySphereTextureNode->SetTexture(&mSkySphereTexture);

	mHasTexturesNode.push_back(mSceneManager->CreateConstantBufferNode<HasTextureBuffer>(4));

	mPlaneTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mPlaneTextureNode->SetName("PlaneTexture");
	mPlaneTextureNode->SetIndex(0);
	mPlaneTextureNode->SetTexture(&mPlaneTexture);
	mHasTexturesNode[0]->GetData().hasTextures.x = 1.0f;

	mRasterizerStateNode = mSceneManager->CreateRasterizerNode();
	mRasterizerStateNode->SetName("RasterizerState");
	mRasterizerStateNode->SetRasterizerStateNode(&mRasterizerState);
	mRasterizerStateNode->SetEnabled(false);

	const uint32_t depthMapSize = 4096;
	mDepthMap.Initialize(depthMapSize, depthMapSize);

	mRenderTarget = new Graphics::RenderTarget();
	mRenderTarget->Initialize(w, h, Graphics::ColorFormat::RGBA_U32);

	mDepthMapNode = mSceneManager->CreateDepthMapNode(4);
	mDepthMapNode->SetName("DepthMap");
	mDepthMapNode->SetDepthMap(&mDepthMap);

	mDepthMapNode2ndPass = mSceneManager->CreateDepthMapNode(4);
	mDepthMapNode2ndPass->SetName("DepthMap2ndPass");
	mDepthMapNode2ndPass->SetDepthMap(&mDepthMap);
	mDepthMapNode2ndPass->SetHasTexture(true);

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

	mSkySphereTransformNode = mSceneManager->CreateTransformNode();
	mSkySphereTransformNode->SetName("SkySphereTransform");
	mSkySphereTransformNode->GetTransform().SetScale({ 4000.0f, 4000.0f, 4000.0f });

	mPlaneTransformNode = mSceneManager->CreateTransformNode();
	mPlaneTransformNode->SetName("Transform");
	mPlaneTransformNode->GetTransform().SetPosition({ -200.0f, 0.0f, -150.0f });
	mPlaneTransformNode->GetTransform().SetScale({ 500.0f, 500.0f, 1.0f });
	mPlaneTransformNode->GetTransform().SetRotation({ 90.0f, 0.0f, 0.0f });

	mSkySphereMeshNode = mSceneManager->CreateMeshNode();
	mSkySphereMeshNode->SetName("SkySphereMesh");
	mSkySphereMeshNode->SetMesh(&mSkySphereMeshBuffer);

	mPlaneMeshNode = mSceneManager->CreateMeshNode();
	mPlaneMeshNode->SetMesh(&mPlaneMeshBuffer);

	mPlaneMaterialNode = mSceneManager->CreateMaterialNode();
	mPlaneMaterialNode->GetMaterial().ambient = Maths::Vector4::Black();
	mPlaneMaterialNode->GetMaterial().diffuse = Maths::Vector4::White();
	mPlaneMaterialNode->GetMaterial().specular = Maths::Vector4::Gray();
	mPlaneMaterialNode->GetMaterial().emissive = Maths::Vector4::Black();
	mPlaneMaterialNode->GetMaterial().power = 1.0f;

	mWarrokModelNode = mSceneManager->CreateModelNodeWithBones("Warrok.wow");
	mWarrokModelNode->SetName("WarrokModel");
	mWarrokModelNode->GetModel().GetBoneWorldTransforms().resize(mWarrokModelNode->GetModel().GetNumberOfBones());

	mWarrokAnimationController.Initialize(mWarrokModelNode->GetModel());
	mWarrokAnimationController.LoadAnimation("SambaDancing.wowanim");
	mWarrokAnimationController.LoadAnimation("HipHopDancing.wowanim");

	mWarrokModelNode->GetTransformNode().GetTransform().SetPosition({ -100.0f, 0.0f, 20.0f });

	mPirateModelNode = mSceneManager->CreateModelNodeWithBones("Pirate.wow");
	mPirateModelNode->SetName("PirateModel");
	mPirateModelNode->GetModel().GetBoneWorldTransforms().resize(mPirateModelNode->GetModel().GetNumberOfBones());

	mPirateAnimationController.Initialize(mPirateModelNode->GetModel());
	mPirateAnimationController.LoadAnimation("Bellydancing.wowanim");

	mPirateModelNode->GetTransformNode().GetTransform().SetPosition({ 100.0f, 0.0f, 0.0f });

	mSceneManager->GetRoot().AddChild(mCameraNode);

	mCameraNode->AddChild(mDepthMapNode);
	mDepthMapNode->AddChild(mDepthMapShaderNode);
	mDepthMapShaderNode->AddChild(mWarrokModelNode);
	mDepthMapShaderNode->AddChild(mPirateModelNode);

	mCameraNode->AddChild(mDepthMapNode2ndPass);
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

	mSamplerNode->AddChild(mPlaneTransformNode);
	mPlaneTransformNode->AddChild(mPlaneMaterialNode);
	mPlaneMaterialNode->AddChild(mHasTexturesNode[0]);
	mHasTexturesNode[0]->AddChild(mPlaneTextureNode);
	mPlaneTextureNode->AddChild(mPlaneMeshNode);

	mSamplerNode->AddChild(mWarrokModelNode);
	mSamplerNode->AddChild(mPirateModelNode);
}

void GameApp::OnTerminate()
{
	mCameraNode->Terminate();
	mLightNode->Terminate();

	mSamplerNode->Terminate();
	mDepthMapNode->Terminate();

	mSceneManager->Purge();

	Graphics::SimpleDraw::Terminate();

	mSkySphereVertexShader.Terminate();
	mSkySpherePixelShader.Terminate();

	mDepthVertexShader.Terminate();

	mShadowMappingVertexShader.Terminate();
	mShadowMappingPixelShader.Terminate();

	mPlaneTexture.Terminate();
	mSkySphereTexture.Terminate();
	mPlane.Destroy();
	mSkySphere.Destroy();
	mPlaneMeshBuffer.Terminate();

	mRenderTarget->Terminate();
	mDepthMap.Terminate();
	SafeDelete(mRenderTarget);

	mSkySphereMeshBuffer.Terminate();

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

	mSkySphereTransformNode->GetTransform().SetPosition(mCamera.GetPosition());

	mCamera.UpdateViewMatrix();
	mLightCamera.UpdateViewMatrix();
	
	if (inputSystem->IsKeyPressed(Input::KeyCode::ONE))
	{
		mWarrokAnimationController.Reset("SambaDancing");
		mPirateAnimationController.Reset("Bellydancing");
		danceSelector = 1;
	}
	else if (inputSystem->IsKeyPressed(Input::KeyCode::TWO))
	{
		mWarrokAnimationController.Reset("HipHopDancing");
		danceSelector = 2;
	}
	else if (inputSystem->IsKeyPressed(Input::KeyCode::ZERO))
	{
		mWarrokAnimationController.ResetAll();
		danceSelector = 0;
	}

	if (danceSelector == 1)
	{
		mWarrokAnimationController.Play("SambaDancing", deltaTime, true);
		mPirateAnimationController.Play("Bellydancing", deltaTime, true);
	}
	else if (danceSelector == 2)
	{
		mWarrokAnimationController.Play("HipHopDancing", deltaTime, true);
	}
	mWarrokAnimationController.Update();
	mPirateAnimationController.Update();

	// Draw mesh

	mSceneManager->Update(deltaTime);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();
	mSceneManager->Render();

	graphicsSystem->ClearDepthBuffer();

	UpdateIMGUI(deltaTime);

	if (toggleBones)
	{
		UpdateSimpleDraw(mCamera);
	}


	graphicsSystem->EndRender();

}

void GameApp::UpdateIMGUI(float deltaTime)
{
	Gui::BeginRender(deltaTime);
	
	ImGui::SetNextWindowSize({ 300.0f, 400.0f });
	ImGui::Begin("Warrok");
	
	ImGui::Text("Press 1 For Samba\nPress 2 For Hip-Hop\nPress 0 to stop");

	ImGui::Checkbox("Toggle Bones", &toggleBones);

	ImGui::End();
	
	Gui::EndRender();
}

void GameApp::UpdateSimpleDraw(WOWGE::Graphics::Camera& camera)
{
	for (uint32_t i{ 0 }; i < mWarrokModelNode->GetModel().GetNumberOfBones(); ++i)
	{
		Graphics::SimpleDraw::AddSphere(Maths::GetTranslation(mWarrokModelNode->GetModel().GetBoneWorldTransforms()[i]), 1.0f, Maths::Vector4::Red(), 20, 20);

		Graphics::Bone* bone{ mWarrokModelNode->GetModel().GetBones()[i] };
		while (bone->parent)
		{
			Graphics::SimpleDraw::AddLine(Maths::GetTranslation(mWarrokModelNode->GetModel().GetBoneWorldTransforms()[bone->index]), Maths::GetTranslation(mWarrokModelNode->GetModel().GetBoneWorldTransforms()[bone->parent->index]), Maths::Vector4::Blue());

			bone = bone->parent;
		}
	}

	Graphics::SimpleDraw::Render(camera);
}