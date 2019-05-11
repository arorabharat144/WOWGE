#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

using namespace WOWGE;

namespace
{
	const uint32_t w = 1920;
	const uint32_t h = 1080;

	float speed = 10.0f;
	float rotationSpeed = 10.0f;
	float modelRotationSpeedX = 0.0f;
	float modelRotationSpeedY = 0.0f;

	uint32_t id = 0;

	Maths::Vector4 lightDirection{ 1.0f, -1.0f, 1.0f, 1.0f };

	float maxAgentSpeed{ 10.0f };
	float mass{ 1.0f };

	Maths::Vector3 destination{ 0.0f, 10.0f, 0.0f };
	Maths::Quaternion modelRotation;
	float playerSpeed{ 100.0f };
	float radius{ 20.0f };

	Maths::Matrix44 correctionMatrix;

	int behaviour{ 0 };
	bool obstacleAvoidance{ false };
	bool wallAvoidance{ false };
}

GameApp::GameApp() :
	mCameraNode(nullptr),
	mLightNode(nullptr),
	mPlaneMaterialNode(nullptr),
	mPlaneMeshNode(nullptr),
	mSkySphereMeshNode(nullptr),
	mModelNode(nullptr),
	mEnemyModelNode(nullptr),
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
	mSkySphereTransformNode(nullptr),
	ship{ aiWorld }
{
	mSceneManager = new WOWGE::Graphics::SceneManager(mMatrixStack);
}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{

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

	mRasterizerState.Initialize(Graphics::CullMode::None, Graphics::FillMode::Wireframe);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience
	mSkySphereVertexShader.Initialize(L"../../Assets/Shaders/SkySphere.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mDepthVertexShader.Initialize(L"../../Assets/Shaders/DepthMap.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mShadowMappingVertexShader.Initialize(L"../../Assets/Shaders/ShadowMapping.fx", "VS", "vs_5_0", Graphics::BoneVertex::Format);
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
	mLightNode->GetLight().diffuse = Maths::Vector4::Gray();
	mLightNode->GetLight().direction = { -1.0f, -1.0f, 1.0f, 0.0f };
	mLightNode->GetLight().specular = Maths::Vector4::White();

	mSkySphereSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSkySphereSamplerNode->SetName("SkySphereSampler");

	mSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap, 0);
	mSamplerNode->SetName("Sampler");

	mDepthSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp, 1);
	mDepthSamplerNode->SetName("DepthSampler");

	mSkySphereTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mSkySphereTextureNode->SetName("SkySphereTexture");
	mSkySphereTextureNode->SetTexture(&mSkySphereTexture);

	mHasTexturesNode.emplace_back(mSceneManager->CreateConstantBufferNode<HasTextureBuffer>(4));

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

	mModelNode = mSceneManager->CreateModelNodeWithBones("Ship.wow");
	mModelNode->SetName("ShipModel");
	mModelNode->GetTransformNode().GetTransform().SetPosition({ -100.0f, 10.0f, 0.0f });
	mModelNode->GetTransformNode().GetTransform().SetRotation({ 90.0f, -90.0f, 0.0f });
	mModelNode->GetTransformNode().GetTransform().SetScale({ 50.0f, 50.0f, 50.0f });

	//mModelNode->GetModel().GetBoneWorldTransforms().resize(mModelNode->GetModel().getnum)

	mEnemyModelNode = mSceneManager->CreateModelNodeWithBones("Ship.wow");
	mEnemyModelNode->SetName("EnemyShipModel");
	mEnemyModelNode->GetTransformNode().GetTransform().SetPosition({ 0.0f, 10.0f, 0.0f });
	mEnemyModelNode->GetTransformNode().GetTransform().SetRotation({ 90.0f, 90.0f, 0.0f });
	mEnemyModelNode->GetTransformNode().GetTransform().SetScale({ 50.0f, 50.0f, 50.0f });

	correctionMatrix = mEnemyModelNode->GetTransformNode().GetTransform().GetTransformMatrix();

	mSceneManager->GetRoot().AddChild(mCameraNode);

	mCameraNode->AddChild(mDepthMapNode);
	mDepthMapNode->AddChild(mDepthMapShaderNode);
	mDepthMapShaderNode->AddChild(mModelNode);
	mDepthMapShaderNode->AddChild(mEnemyModelNode);

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
	mPlaneMaterialNode->AddChild(mPlaneTextureNode);
	mPlaneTextureNode->AddChild(mHasTexturesNode[0]);
	mHasTexturesNode[0]->AddChild(mPlaneMeshNode);

	mSamplerNode->AddChild(mModelNode);
	mSamplerNode->AddChild(mEnemyModelNode);

	ship.Initialize(mModelNode->GetTransformNode().GetTransform().GetPosition(), maxAgentSpeed, mass);
	ship.Radius() = radius;

	aiWorld.AddObstacles({ {0.0f, 30.0f, 0.0f}, 30.0f });
	aiWorld.AddObstacles({ { -100.0f, 30.0f, 10.0f }, 30.0f });
	aiWorld.AddObstacles({ { 0.0f, 30.0f, 200.0f }, 25.0f });
	aiWorld.AddObstacles({ { 200.0f, 30.0f, 100.0f }, 50.0f });
	aiWorld.AddObstacles({ {250.0f, 30.0f, -50.0f}, 45.0f });

	aiWorld.AddWall({ {-200.0f, 0.0f, 100.0f}, {10.0f, 100.0f, 250.0f} });
	aiWorld.AddWall({ { 300.0f, 0.0f, 100.0f },{ 10.0f, 100.0f, 250.0f } });

	aiWorld.AddWall({ { 50.0f, 0.0f, 350.0f },{ 250.0f, 100.0f, 10.0f } });
	aiWorld.AddWall({ { 50.0f, 0.0f, -150.0f },{ 250.0f, 100.0f, 10.0f } });
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

	mSkySphereTransformNode->GetTransform().SetPosition(mCamera.GetPosition());

	mCamera.UpdateViewMatrix();
	mLightCamera.UpdateViewMatrix();

	// Draw mesh
	mModelNode->GetTransformNode().GetTransform().SetPosition({ destination.x, mModelNode->GetTransformNode().GetTransform().GetPosition().y, destination.z });
	ship.Destination() = destination + Maths::Vector3(0.0f, 0.0f, -mModelNode->GetTransformNode().GetTransform().GetScale().z);
	ship.Update(deltaTime);

	mEnemyModelNode->GetTransformNode().GetTransform().SetTransformMatrix(correctionMatrix * ship.LocalToWorld());
	mEnemyModelNode->GetTransformNode().GetTransform().SetScale(Maths::GetScale(correctionMatrix));
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
	ImGui::SetNextWindowPos({ 0.0f, static_cast<float>(h) - 500.0f });
	ImGui::Begin("Agent");
	
	ImGui::SliderFloat("Mass", &mass, 1.0f, 10.0f);
	ship.GetMass() = mass;

	ImGui::SliderFloat("Speed", &maxAgentSpeed, 1.0f, 500.0f);
	ship.MaxSpeed() = maxAgentSpeed;

	ImGui::SliderFloat("Arrive Slowing Radius", &ship.GetSteeringModule().GetBehaviour<AI::ArriveBehaviour>("Arrive")->GetSlowingRadius(), 100.0f, 1000.0f);
	ImGui::SliderFloat("Wander Circle Distance", &ship.GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->GetCircleDistance(), 1.0f, 100.0f);
	ImGui::SliderFloat("Wander Circle Radius", &ship.GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->GetCircleRadius(), 1.0f, 100.0f);
	ImGui::SliderFloat("Wander Jitter", &ship.GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->GetWanderJitter(), 0.0f, 50.0f);
	ImGui::SliderFloat("Obstacle Avoidance\n MaxAvoidForce", &ship.GetSteeringModule().GetBehaviour<AI::ObstacleAvoidanceBehaviour>("ObstacleAvoidance")->GetMaxAvoidForce(), 100.0f, 1000.0f);

	ImGui::End();

	ImGui::SetNextWindowPos({ 500.0f, static_cast<float>(h) - 500.0f });
	ImGui::Begin("Steering Behaviours");

	ImGui::RadioButton("Seek", &behaviour, 0);
	ImGui::RadioButton("Flee", &behaviour, 1);
	ImGui::RadioButton("Arrive", &behaviour, 2);
	ImGui::RadioButton("Pursuit", &behaviour, 3);
	ImGui::RadioButton("Evade", &behaviour, 4);
	ImGui::RadioButton("Wander", &behaviour, 5);

	ImGui::Checkbox("Obstacle Avoidance", &obstacleAvoidance);
	//ImGui::Checkbox("Wall Avoidance", &wallAvoidance);

	if (obstacleAvoidance)
	{
		ship.GetSteeringModule().GetBehaviour<AI::ObstacleAvoidanceBehaviour>("ObstacleAvoidance")->SetActive(true);
	}

	if (wallAvoidance)
	{
		ship.GetSteeringModule().GetBehaviour<AI::WallAvoidanceBehaviour>("WallAvoidance")->SetActive(true);
	}

	if (behaviour == 0)
	{
		ship.GetSteeringModule().GetBehaviour<AI::SeekBehaviour>("Seek")->SetActive(true);
		ship.GetSteeringModule().GetBehaviour<AI::FleeBehaviour>("Flee")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::ArriveBehaviour>("Arrive")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::PursuitBehaviour>("Pursuit")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Evade")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Wander")->SetActive(false);
	}

	if (behaviour == 1)
	{
		ship.GetSteeringModule().GetBehaviour<AI::SeekBehaviour>("Seek")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::FleeBehaviour>("Flee")->SetActive(true);
		ship.GetSteeringModule().GetBehaviour<AI::ArriveBehaviour>("Arrive")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::PursuitBehaviour>("Pursuit")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Evade")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Wander")->SetActive(false);
	}

	if (behaviour == 2)
	{
		ship.GetSteeringModule().GetBehaviour<AI::SeekBehaviour>("Seek")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::FleeBehaviour>("Flee")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::ArriveBehaviour>("Arrive")->SetActive(true);
		ship.GetSteeringModule().GetBehaviour<AI::PursuitBehaviour>("Pursuit")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Evade")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Wander")->SetActive(false);
	}

	if (behaviour == 3)
	{
		ship.GetSteeringModule().GetBehaviour<AI::SeekBehaviour>("Seek")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::FleeBehaviour>("Flee")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::ArriveBehaviour>("Arrive")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::PursuitBehaviour>("Pursuit")->SetActive(true);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Evade")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Wander")->SetActive(false);
	}

	if (behaviour == 4)
	{
		ship.GetSteeringModule().GetBehaviour<AI::SeekBehaviour>("Seek")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::FleeBehaviour>("Flee")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::ArriveBehaviour>("Arrive")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::PursuitBehaviour>("Pursuit")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Evade")->SetActive(true);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Wander")->SetActive(false);
	}

	if (behaviour == 5)
	{
		ship.GetSteeringModule().GetBehaviour<AI::SeekBehaviour>("Seek")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::FleeBehaviour>("Flee")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::ArriveBehaviour>("Arrive")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::PursuitBehaviour>("Pursuit")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Evade")->SetActive(false);
		ship.GetSteeringModule().GetBehaviour<AI::EvadeBehaviour>("Wander")->SetActive(true);
	}

	ImGui::End();

	Gui::EndRender();
}

void GameApp::UpdateSimpleDraw(const WOWGE::Graphics::Camera& camera)
{
	//Graphics::SimpleDraw::AddLine(ship.Position(), (ship.Position() + (ship.Heading() * 20.0f)), Maths::Vector4::Green());

	Graphics::SimpleDraw::AddSphere(ship.Destination(), 5.0f, Maths::Vector4::Blue(), 20, 20);
	Graphics::SimpleDraw::AddSphere(ship.Destination(), ship.GetSteeringModule().GetBehaviour<AI::ArriveBehaviour>("Arrive")->GetSlowingRadius(), Maths::Vector4::Red(), 20, 20);

	Graphics::SimpleDraw::AddSphere((ship.Position() + (ship.Heading() * ship.GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->GetCircleDistance())), ship.GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->GetCircleRadius(), Maths::Vector4::Magenta(), 10, 10);

	Graphics::SimpleDraw::AddSphere(ship.GetSteeringModule().GetBehaviour<AI::WanderBehaviour>("Wander")->GetTargetToFollowInWorldSpace(), 2.0f, Maths::Vector4::Black(), 20, 20);

	//Graphics::SimpleDraw::AddLine(ship.GetSteeringModule().GetBehaviour<AI::ObstacleAvoidanceBehaviour>("ObstacleAvoidance")->ahead, ship.Position(), Maths::Vector4::Red());

	Graphics::SimpleDraw::AddSphere(ship.Position(), radius, Maths::Vector4::Black());

	for (uint32_t i{ 0 }; i < aiWorld.GetObstacles().size(); ++i)
	{
		Graphics::SimpleDraw::AddSphere(aiWorld.GetObstacles()[i].center, aiWorld.GetObstacles()[i].radius, Maths::Vector4::Cyan());
	}

	for (uint32_t i{ 0 }; i < aiWorld.GetWalls().size(); ++i)
	{
		Graphics::SimpleDraw::AddAABB(aiWorld.GetWalls()[i], Maths::Vector4::LightRed());
	}

	Graphics::SimpleDraw::Render(camera);
}