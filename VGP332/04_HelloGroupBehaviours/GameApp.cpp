#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

using namespace WOWGE;

namespace
{
	float speed = 10.0f;
	float rotationSpeed = 10.0f;

	uint32_t id = 0;

	Maths::Vector4 lightDirection{ 1.0f, -1.0f, 1.0f, 1.0f };

	const uint32_t numberOfCylinders{ 100 };
	Maths::Matrix44 correctionMatrix[numberOfCylinders];

	Maths::Vector3 destination;

	float playerSpeed{ 500.0f };

	float maxSpeed{ 10.0f };
	float mass{ 1.0f };

	float radius{ 10.0f };

	float wanderCircleDistance{ 15.0f };
	float wanderCircleRadius{ 50.0f };
	float wanderJitter{ 2.0f };

	bool separation{ false };
	bool alignment{ false };
	bool cohesion{ false };
	bool wander{ false };
	bool seek{ false };
}

GameApp::GameApp() :
	mCameraNode(nullptr),
	mLightNode(nullptr),
	mPlaneMaterialNode(nullptr),
	mPlaneMeshNode(nullptr),
	mSkySphereMeshNode(nullptr),
	mModelNode(nullptr),
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

	const float widthLightCamera = w * 0.2f;
	const float heightLightCamera = h * 0.2f;

	mWindow.Initialize(GetInstance(), GetAppName(), w, h);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	Graphics::MeshBuilder::CreateSkySphere(mSkySphere, 200, 200);
	mSkySphereMeshBuffer.Initialize(mSkySphere);
	mSkySphereTexture.Initialize("../../Assets/Images/Stars.jpg");

	Graphics::MeshBuilder::CreatePlane(mPlane);
	mPlaneMeshBuffer.Initialize(mPlane);
	mPlaneTexture.Initialize("../../Assets/Images/BrickSmallBrown.jpg");

	Graphics::MeshBuilder::CreateCylinder(mCylinder, 20, 20);
	mCylinderMeshBuffer.Initialize(mCylinder);

	mCylinderMeshNode = mSceneManager->CreateMeshNode();
	mCylinderMeshNode->SetName("CylinderMesh");
	mCylinderMeshNode->SetMesh(&mCylinderMeshBuffer);

	mCylinderMaterialsNodes.resize(numberOfCylinders);
	mCylinderTransformsNodes.resize(numberOfCylinders);
	for (uint32_t i{ 0 }; i < numberOfCylinders; ++i)
	{
		mCylinderMaterialsNodes[i] = mSceneManager->CreateMaterialNode();
		mCylinderMaterialsNodes[i]->SetName("CylinderMaterial");
		mCylinderMaterialsNodes[i]->GetMaterial().ambient = Maths::Vector4::Black();
		mCylinderMaterialsNodes[i]->GetMaterial().diffuse = 
		{
			std::uniform_real_distribution<float>{0.0f, 1.0f}(engine),
			std::uniform_real_distribution<float>{0.0f, 1.0f}(engine),
			std::uniform_real_distribution<float>{0.0f, 1.0f}(engine),
			1.0f
		};
		mCylinderMaterialsNodes[i]->GetMaterial().specular = Maths::Vector4::Gray();
		mCylinderMaterialsNodes[i]->GetMaterial().emissive = Maths::Vector4::Black();
		mCylinderMaterialsNodes[i]->GetMaterial().power = 10.0f;

		mCylinderTransformsNodes[i] = mSceneManager->CreateTransformNode();
		mCylinderTransformsNodes[i]->SetName("CylinderTransform");
		mCylinderTransformsNodes[i]->GetTransform().SetScale({ 5.0f, 10.0f, 5.0f });
		mCylinderTransformsNodes[i]->GetTransform().SetRotation({ 90.0f, 0.0f, 0.0f });
		correctionMatrix[i] = mCylinderTransformsNodes[i]->GetTransform().GetTransformMatrix();

		mCylinderTransformsNodes[i]->GetTransform().SetPosition(
			{
				std::uniform_real_distribution<float>{-100.0f, 100.0f}(engine),
				0.0f,
				std::uniform_real_distribution<float>{-100.0f, 100.0f}(engine)
			});
	}

	mRasterizerState.Initialize(Graphics::CullMode::None, Graphics::FillMode::Wireframe);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience
	mSkySphereVertexShader.Initialize(L"../../Assets/Shaders/SkySphere.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mDepthVertexShader.Initialize(L"../../Assets/Shaders/DepthMap.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mShadowMappingVertexShader.Initialize(L"../../Assets/Shaders/ShadowMapping.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	///////////////////////////////////////
	// Compile and create the pixel shader

	mSkySpherePixelShader.Initialize(L"../../Assets/Shaders/SkySphere.fx");
	mShadowMappingPixelShader.Initialize(L"../../Assets/Shaders/ShadowMapping.fx");

	mCamera.Initialize(0.785f, (float)w, (float)h, 1.0f, 5000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.Pitch(15.0f);
	mCamera.SetPosition({ 0.0f, 100.0f,-500.0f });
	mTimer.Initialize();

	mLightCamera.Initialize(0.785f, (float)w, (float)h, 25.0f, 500.0f);
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

	mSkySphereSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSkySphereSamplerNode->SetName("SkySphereSampler");

	mSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSamplerNode->SetName("Sampler");

	mDepthSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp, 1);
	mDepthSamplerNode->SetName("DepthSampler");

	mSkySphereTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mSkySphereTextureNode->SetName("SkySphereTexture");
	mSkySphereTextureNode->SetTexture(&mSkySphereTexture);

	mHasTexturesNode.emplace_back(mSceneManager->CreateConstantBufferNode<HasTextureBuffer>(4));

	mPlaneTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mPlaneTextureNode->SetName("PlaneTexture");
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

	mModelNode = mSceneManager->CreateModelNodeWithBones("Ship.wow");
	mModelNode->SetName("ShipModel");
	mModelNode->GetTransformNode().GetTransform().SetPosition({ -100.0f, 10.0f, 0.0f });
	mModelNode->GetTransformNode().GetTransform().SetRotation({ 90.0f, -90.0f, 0.0f });
	mModelNode->GetTransformNode().GetTransform().SetScale({ 50.0f, 50.0f, 50.0f });

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
	mPlaneMaterialNode->GetMaterial().power = 10.0f;

	mHasTexturesNode.emplace_back(mSceneManager->CreateConstantBufferNode<HasTextureBuffer>(4));

	mSceneManager->GetRoot().AddChild(mCameraNode);

	mCameraNode->AddChild(mDepthMapNode);
	mDepthMapNode->AddChild(mDepthMapShaderNode);
	mDepthMapShaderNode->AddChild(mModelNode);
	for (uint32_t i{ 0 }; i < numberOfCylinders; ++i)
	{
		mDepthMapShaderNode->AddChild(mCylinderTransformsNodes[i]);
		mCylinderTransformsNodes[i]->AddChild(mCylinderMaterialsNodes[i]);
		mCylinderMaterialsNodes[i]->AddChild(mHasTexturesNode[1]);
	}
	mHasTexturesNode[1]->AddChild(mCylinderMeshNode);

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

	mSamplerNode->AddChild(mModelNode);
	for (uint32_t i{ 0 }; i < numberOfCylinders; ++i)
	{
		mSamplerNode->AddChild(mCylinderTransformsNodes[i]);
	}

	mEntities.resize(numberOfCylinders);
	for (uint32_t i{ 0 }; i < numberOfCylinders; ++i)
	{
		mEntities[i] = std::make_unique<Cylinder>(mAIWorld);
		mEntities[i]->Initialize(mCylinderTransformsNodes[i]->GetTransform().GetPosition(), maxSpeed, mass);
		mEntities[i]->Radius() = radius;
	}

	Graphics::SimpleDraw::Initialize();
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
	mCylinder.Destroy();
	mCylinderMeshBuffer.Terminate();
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
	else
	{
		if (inputSystem->IsKeyDown(Input::KeyCode::W))
		{
			destination.z += playerSpeed * deltaTime;
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::S))
		{
			destination.z -= playerSpeed * deltaTime;
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::A))
		{
			destination.x -= playerSpeed * deltaTime;
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::D))
		{
			destination.x += playerSpeed * deltaTime;
		}
	}
	if (inputSystem->IsKeyPressed(Input::KeyCode::F1))
	{
		mRasterizerStateNode->SetEnabled(!mRasterizerStateNode->GetEnabled());
	}

	mModelNode->GetTransformNode().GetTransform().SetPosition({ destination.x, mModelNode->GetTransformNode().GetTransform().GetPosition().y, destination.z });

	mSkySphereTransformNode->GetTransform().SetPosition(mCamera.GetPosition());

	mCamera.UpdateViewMatrix();
	mLightCamera.UpdateViewMatrix();

	// Draw mesh

	for (uint32_t i{ 0 }; i < numberOfCylinders; ++i)
	{
		mEntities[i]->Destination() = destination + Maths::Vector3(0.0f, 0.0f, -mModelNode->GetTransformNode().GetTransform().GetScale().z);

		mEntities[i]->Update(deltaTime);
		mCylinderTransformsNodes[i]->GetTransform().SetTransformMatrix(correctionMatrix[i] * mEntities[i]->LocalToWorld());
		mCylinderTransformsNodes[i]->GetTransform().SetScale(Maths::GetScale(correctionMatrix[i]));
	}

	mSceneManager->Update(deltaTime);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();
	mSceneManager->Render();

	UpdateSimpleDraw(mCamera);

	UpdateIMGUI(deltaTime);

	graphicsSystem->EndRender();
}

void GameApp::UpdateIMGUI(float deltaTime)
{
	Gui::BeginRender(deltaTime);
	ImGui::SetNextWindowSize({ 500.0f, 400.0f });
	ImGui::Begin("GroupBehaviours");

	ImGui::Text("FPS: %f", mTimer.GetFPS());
	ImGui::SliderFloat("Mass", &mass, 1.0f, 10.0f);
	ImGui::SliderFloat("Speed", &maxSpeed, 1.0f, 1000.0f);

	ImGui::SliderFloat("Wander Circle Distance", &wanderCircleDistance, 1.0f, 100.0f);
	ImGui::SliderFloat("Wander Circle Radius", &wanderCircleRadius, 1.0f, 100.0f);
	ImGui::SliderFloat("Wander Jitter", &wanderJitter, 0.0f, 50.0f);

	ImGui::Checkbox("Separation", &separation);
	ImGui::Checkbox("Alignment", &alignment);
	ImGui::Checkbox("Cohesion", &cohesion);
	ImGui::Checkbox("Wander", &wander);
	ImGui::Checkbox("Seek", &seek);

	for (uint32_t i{ 0 }; i < numberOfCylinders; ++i)
	{
		mEntities[i]->GetMass() = mass;
		mEntities[i]->MaxSpeed() = maxSpeed;

		mEntities[i]->GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->GetCircleDistance() = wanderCircleDistance;
		mEntities[i]->GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->GetCircleRadius() = wanderCircleRadius;
		mEntities[i]->GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->GetWanderJitter() = wanderJitter;
		
		mEntities[i]->GetSteeringModule().GetBehaviour<AI::Separation>("Separation")->SetActive(separation);
		mEntities[i]->GetSteeringModule().GetBehaviour<AI::Alignment>("Alignment")->SetActive(alignment);
		mEntities[i]->GetSteeringModule().GetBehaviour<AI::Cohesion>("Cohesion")->SetActive(cohesion);
		mEntities[i]->GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->SetActive(wander);
		mEntities[i]->GetSteeringModule().GetBehaviour<AI::SeekBehaviour>("Seek")->SetActive(seek);
	}

	ImGui::End();
	Gui::EndRender();
}

void GameApp::UpdateSimpleDraw(const WOWGE::Graphics::Camera& camera)
{
	for (uint32_t i{ 0 }; i < numberOfCylinders; ++i)
	{
		//Graphics::SimpleDraw::AddSphere(mEntities[i]->Position(), mEntities[i]->Radius(), Maths::Vector4::Red());
	}
	Graphics::SimpleDraw::Render(camera);
}