#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

#include <fstream>

#include "WorldManager.h"

#include "StateTypes.h"

using namespace WOWGE;

using namespace DirectX;

namespace
{
	bool debugMode{ false };
	float speed = 50.0f;
	float rotationSpeed = 10.0f;

	Maths::Vector4 lightDirection{ 1.0f, -1.0f, 1.0f, 1.0f };

	bool toggleBones{ false };

	float pos{ 0.0f };

	const uint32_t w = 1920;
	const uint32_t h = 1080;

	Maths::Vector3 point1{ FLT_MAX, FLT_MAX, FLT_MAX };
	Maths::Vector3 point2{ FLT_MAX, FLT_MAX, FLT_MAX };

	Maths::Ray ray;

	std::vector<Maths::AABB> walls;

	//float cellSize{ 20.0f };

	enum class GUISelector
	{
		NONE = -1,
		WALLS,
		BLOCKEDCOORDS,
		DESTINATION
	};

	int selection{ -1 };

	char destinationName[64];
	Maths::Vector3 destinationNormal;
}

GameApp::GameApp() :
	mCameraNode(nullptr),
	mLightNode(nullptr),
	mSkySphereMeshNode(nullptr),
	mOfficeModelNode(nullptr),
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
	mSkySphereTextureNode(nullptr),
	mSkySphereTransformNode(nullptr)
{
	mSceneManager = new WOWGE::Graphics::SceneManager(mMatrixStack);
}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	WorldManager::StaticInitialize();
	WorldManager::Get()->Initialize();

	LoadDestinations("Destinations.data");

	const float widthLightCamera = w * 0.2f;
	const float heightLightCamera = h * 0.2f;

	mWindow.Initialize(GetInstance(), GetAppName(), w, h);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	Graphics::SimpleDraw::Initialize();

	Graphics::MeshBuilder::CreateSkySphere(mSkySphere, 200, 200);
	mSkySphereMeshBuffer.Initialize(mSkySphere);
	mSkySphereTexture.Initialize("../../Assets/Images/Stars.jpg");

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

	mCamera.Initialize(0.785f, (float)w, (float)h, 1.0f, 10000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.Pitch(15.0f);
	mCamera.SetPosition({ 100.0f, 50.0f, 300.0f });
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
	mLightNode->GetLight().ambient = Maths::Vector4::White();
	mLightNode->GetLight().diffuse = Maths::Vector4::White();
	mLightNode->GetLight().direction = { -1.0f, -1.0f, -1.0f, 1.0f };
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
	mSkySphereTransformNode->GetTransform().SetScale({ 5000.0f, 5000.0f, 5000.0f });

	mSkySphereMeshNode = mSceneManager->CreateMeshNode();
	mSkySphereMeshNode->SetName("SkySphereMesh");
	mSkySphereMeshNode->SetMesh(&mSkySphereMeshBuffer);

	mOfficeModelNode = mSceneManager->CreateModelNodeWithBones("Office.wow");
	mOfficeModelNode->SetName("OfficeModel");
	mOfficeModelNode->GetModel().GetBoneWorldTransforms().resize(mOfficeModelNode->GetModel().GetNumberOfBones());

	mOfficeModelNode->GetTransformNode().GetTransform().SetPosition({ 1000.0f, 407.0f, 0.0f });

	//Setting up model names
	modelNames.emplace_back("Remy");
	modelNames.emplace_back("Regina");
	modelNames.emplace_back("Malcolm");
	modelNames.emplace_back("Liam");
	modelNames.emplace_back("Shae");

	officeWorkers.resize(modelNames.size());

	//Setting up animation names
	animationNames.emplace_back("Idle");
	animationNames.emplace_back("Running");
	animationNames.emplace_back("SittingIdle");
	animationNames.emplace_back("SitToStand");
	animationNames.emplace_back("SitToType");
	animationNames.emplace_back("TypeToSit");
	animationNames.emplace_back("StandToSit");
	animationNames.emplace_back("Talking");
	animationNames.emplace_back("Typing");
	animationNames.emplace_back("FilingCabinet");
	animationNames.emplace_back("Walking");

	InitializeOfficeWorkers();

	mSceneManager->GetRoot().AddChild(mCameraNode);

	//Commenting out the shadows
	//mCameraNode->AddChild(mDepthMapNode);
	//mDepthMapNode->AddChild(mDepthMapShaderNode);
	//mDepthMapShaderNode->AddChild(officeWorker->GetModelNode());

	mCameraNode->AddChild(mRasterizerStateNode);
	//mDepthMapNode2ndPass->AddChild(mRasterizerStateNode);
	mRasterizerStateNode->AddChild(mShadowMapShaderNode);
	mShadowMapShaderNode->AddChild(mLightNode);
	mShadowMapShaderNode->AddChild(mDepthSamplerNode);
	mShadowMapShaderNode->AddChild(mSamplerNode);

	//SkySphere Nodes
	mRasterizerStateNode->AddChild(mSkySphereShaderNode);
	mSkySphereShaderNode->AddChild(mSkySphereSamplerNode);
	mSkySphereSamplerNode->AddChild(mSkySphereTransformNode);
	mSkySphereTransformNode->AddChild(mSkySphereTextureNode);
	mSkySphereTextureNode->AddChild(mSkySphereMeshNode);

	mSamplerNode->AddChild(mOfficeModelNode);
	for (uint32_t i{ 0 }; i < officeWorkers.size(); ++i)
	{
		mSamplerNode->AddChild(officeWorkers[i]->GetModelNode());
	}

	AUDIO_ENGINE_FLAGS eFlags{ AudioEngine_Default | AudioEngine_Debug };

	audioEngine = std::make_unique<AudioEngine>(eFlags);

	officeAmbience = std::make_unique<SoundEffect>(audioEngine.get(), L"Post Office Ambience.wav");
	officeAmbienceInstance = officeAmbience->CreateInstance();

	fireAlarm = std::make_unique<SoundEffect>(audioEngine.get(), L"Fire Alarm.wav");
	fireAlarmInstance = fireAlarm->CreateInstance();

	plane.n = { 0.0f, 1.0f, 0.0f };

	LoadWalls("Walls.data");

	for (uint32_t i{ 0 }; i < walls.size(); ++i)
	{
		aiWorld.AddWall(walls[i]);
	}

	aiWorld.GetNavGraph().Init(columns, rows, WorldManager::Get()->CellSize());

	LoadBlockedCoords("BlockedCoords.data");

	for (uint32_t i{ 0 }; i < blockedCoords.size(); ++i)
	{
		aiWorld.GetNavGraph().GetNode(blockedCoords[i]).blocked = true;
	}
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

	mSkySphereTexture.Terminate();
	mSkySphere.Destroy();

	mRenderTarget->Terminate();
	mDepthMap.Terminate();
	SafeDelete(mRenderTarget);

	mSkySphereMeshBuffer.Terminate();

	mRasterizerState.Terminate();

	Gui::Terminate();
	Graphics::GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();
	mWindow.Terminate();

	aiWorld.GetNavGraph().Clear();
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

	mLightNode->GetLight().direction = { mCamera.GetDirection().x, mCamera.GetDirection().y, mCamera.GetDirection().z, 1.0f };

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

	//mSkySphereTransformNode->GetTransform().SetPosition(mCamera.GetPosition());

	mCamera.UpdateViewMatrix();
	mLightCamera.UpdateViewMatrix();

	if (inputSystem->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		int mouseX{ inputSystem->GetMouseScreenX() };
		int mouseY{ inputSystem->GetMouseScreenY() };

		if (selection == static_cast<int>(GUISelector::WALLS))
		{
			if ((point1.x == FLT_MAX && point1.x == FLT_MAX && point1.x == FLT_MAX))
			{

				AddPointMouseClick(mouseX, mouseY, point1);
			}
			else if ((point2.x == FLT_MAX && point2.x == FLT_MAX && point2.x == FLT_MAX))
			{
				AddPointMouseClick(mouseX, mouseY, point2);

				walls.emplace_back(CreateAABB(point1, point2));

				point1 = { FLT_MAX, FLT_MAX, FLT_MAX };
				point2 = { FLT_MAX, FLT_MAX, FLT_MAX };
			}
		}
		else if (selection == static_cast<int>(GUISelector::BLOCKEDCOORDS))
		{
			AddPointMouseClick(mouseX, mouseY, point1);

			SetBlockedCoord(point1);
		}
		else if (selection == static_cast<int>(GUISelector::DESTINATION))
		{
			AddPointMouseClick(mouseX, mouseY, point1);

			SetDestination(point1, "", {0.0f, 0.0f, 0.0f});
		}
	}

	if (inputSystem->IsKeyPressed(Input::KeyCode::F2))
	{
		debugMode = !debugMode;
	}

	for (uint32_t i{ 0 }; i < officeWorkers.size(); ++i)
	{
		officeWorkers[i]->Update(deltaTime);
	}
	// Draw mesh
	if (!audioEngine->Update())
	{
		ASSERT(audioEngine->IsCriticalError(), "No Audio device is active");
	}

	if (!WorldManager::Get()->HasTriggeredAlarm())
	{
		officeAmbienceInstance->Play(true);
	}
	else
	{
		officeAmbienceInstance->Stop();
		fireAlarmInstance->Play(true);
	}

	mSceneManager->Update(deltaTime);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();
	mSceneManager->Render();

	graphicsSystem->ClearDepthBuffer();

	if (debugMode)
	{
		UpdateSimpleDraw(mCamera);
	}

	UpdateIMGUI(deltaTime);
	graphicsSystem->EndRender();

}

void GameApp::UpdateIMGUI(float deltaTime)
{
	Gui::BeginRender(deltaTime);

	ImGui::SetNextWindowSize({ 315.0f, 400.0f });
	ImGui::Begin("Map Editor(Beta)");


	ImGui::RadioButton("Walls", &selection, static_cast<int>(GUISelector::WALLS));
	ImGui::RadioButton("Blocked Coordinates", &selection, static_cast<int>(GUISelector::BLOCKEDCOORDS));
	ImGui::RadioButton("Destination Marker", &selection, static_cast<int>(GUISelector::DESTINATION));

	if (selection == static_cast<int>(GUISelector::DESTINATION))
	{
		ImGui::InputText("Name", destinationName, sizeof(destinationName));

		ImGui::Text("Normal\n");

		ImGui::InputFloat("x", &destinationNormal.x);
		ImGui::InputFloat("y", &destinationNormal.y);
		ImGui::InputFloat("z", &destinationNormal.z);

		if (ImGui::Button("Set Destination"))
		{
			if (strcmp(destinationName, "") != 0)
			{
				WorldManager::Get()->Destinations().back().name = destinationName;
				WorldManager::Get()->Destinations().back().normal = destinationNormal;

				destinationName[0] = '\0';
			}
		}
	}

	if (ImGui::Button("Save Data"))
	{
		if (selection == static_cast<int>(GUISelector::WALLS))
		{
			SaveWalls("Walls.data");
		}
		else if (selection == static_cast<int>(GUISelector::BLOCKEDCOORDS))
		{
			SaveBlockedCoords("BlockedCoords.data");
		}
		else if (selection == static_cast<int>(GUISelector::DESTINATION))
		{
			SaveDestinations("Destinations.data");
		}
	}

	if (ImGui::Button("Start Simulation"))
	{
		WorldManager::Get()->HasStartedSimulation() = true;
	}

	if (WorldManager::Get()->HasStartedSimulation())
	{
		if (ImGui::Button("Trigger Fire Alarm"))
		{
			WorldManager::Get()->HasTriggeredAlarm() = true;
		}
	}

	ImGui::Text("Press F1 to enable/disable backface culling");
	ImGui::Text("Press F2 to enable debug mode");

	ImGui::End();

	Gui::EndRender();
}

void GameApp::UpdateSimpleDraw(WOWGE::Graphics::Camera& camera)
{
	for (uint32_t i{ 0 }; i < officeWorkers.size(); ++i)
	{
		for (uint32_t j{ 0 }; j < officeWorkers[i]->GetPath().size(); ++j)
		{
			if (j == officeWorkers[i]->GetPath().size() - 1)
			{
				break;
			}

			float pointX = static_cast<float>(officeWorkers[i]->GetPath()[j].x);
			float pointZ = static_cast<float>(officeWorkers[i]->GetPath()[j].y);

			float nextpointX = static_cast<float>(officeWorkers[i]->GetPath()[j + 1].x);
			float nextpointZ = static_cast<float>(officeWorkers[i]->GetPath()[j + 1].y);

			Graphics::SimpleDraw::AddLine({ pointX, 0.0f, pointZ }, { nextpointX, 0.0f, nextpointZ }, Maths::Vector4::Blue());
		}

		for (uint32_t j{ 0 }; j < officeWorkers[i]->GetModelNode()->GetModel().GetNumberOfBones(); ++j)
		{
			Graphics::SimpleDraw::AddSphere(Maths::GetTranslation(officeWorkers[i]->GetModelNode()->GetModel().GetBoneWorldTransforms()[j]), 1.0f, Maths::Vector4::Red(), 4, 2);

			Graphics::Bone* bone{ officeWorkers[i]->GetModelNode()->GetModel().GetBones()[j] };
			while (bone->parent)
			{
				Graphics::SimpleDraw::AddLine(Maths::GetTranslation(officeWorkers[i]->GetModelNode()->GetModel().GetBoneWorldTransforms()[bone->index]), Maths::GetTranslation(officeWorkers[i]->GetModelNode()->GetModel().GetBoneWorldTransforms()[bone->parent->index]), Maths::Vector4::Blue());

				bone = bone->parent;
			}
		}
	}

	for (uint32_t y = 0; y < columns; ++y)
	{
		for (uint32_t x = 0; x < rows; ++x)
		{
			float centreX = 2 * x * WorldManager::Get()->CellSize();
			float centreZ = 2 * y * WorldManager::Get()->CellSize();

			Graphics::SimpleDraw::AddSquare({ centreX, 0.0f, centreZ }, WorldManager::Get()->CellSize(), Maths::Vector4::Black(), Graphics::SimpleDraw::Axis::YAxis);

			if (aiWorld.GetNavGraph().GetNode({ x, y }).blocked)
			{
				Graphics::SimpleDraw::AddSphere({ centreX, 0.0f, centreZ }, 10.0f, Maths::Vector4::LightRed(), 4, 2);
			}
		}
	}

	Graphics::SimpleDraw::AddPlane(plane, { 0.0f, 0.0f, 0.0f }, 5000.0f, 100.0f, Maths::Vector4::Red());

	for (uint32_t i{ 0 }; i < officeWorkers.size(); ++i)
	{
		Graphics::SimpleDraw::AddSphere(officeWorkers[i]->Position(), officeWorkers[i]->Radius(), Maths::Vector4::Green(), 4, 2);

		//Graphics::SimpleDraw::AddSphere(officeWorkers[i]->GetSteeringModule().GetBehaviour<AI::WallAvoidanceBehaviour>("WallAvoidance")->point, 1.0f, Maths::Vector4::Magenta());

		for (uint32_t j{ 0 }; j < 3; ++j)
		{
			//Graphics::SimpleDraw::AddLine({ officeWorkers[i]->Position().x, officeWorkers[i]->Position().y + 5.0f,  officeWorkers[i]->Position().z }, officeWorkers[i]->GetSteeringModule().GetBehaviour<AI::WallAvoidanceBehaviour>("WallAvoidance")->mFeelers[i], Maths::Vector4::Blue());
		}
	}
	//for (uint32_t i{ 0 }; i < officeWorker->WallsInRange().size(); ++i)
	//{
	//	Graphics::SimpleDraw::AddAABB(officeWorker->WallsInRange()[i], Maths::Vector4::Red());
	//}

	for (uint32_t i{ 0 }; i < walls.size(); ++i)
	{
		Graphics::SimpleDraw::AddAABB(walls[i], Maths::Vector4::Cyan());
	}



	for (uint32_t i{ 0 }; i < WorldManager::Get()->Destinations().size(); ++i)
	{
		float centreX = 2 * WorldManager::Get()->Destinations()[i].coordinate.x * WorldManager::Get()->CellSize();
		float centreZ = 2 * WorldManager::Get()->Destinations()[i].coordinate.y * WorldManager::Get()->CellSize();
		Graphics::SimpleDraw::AddSphere({ centreX, 0.0f, centreZ }, 2.0f, Maths::Vector4::Green(), 4, 2);
	}

	Graphics::SimpleDraw::Render(camera);
}

void GameApp::AddPointMouseClick(int mouseX, int mouseY, WOWGE::Maths::Vector3& point)
{
	ray = mCamera.ScreenPointToRay(mouseX, mouseY, w, h);

	Maths::Vector3 contactPoint;

	if (Maths::Intersect(ray, plane, contactPoint))
	{
		point = contactPoint;
	}
}

Maths::AABB GameApp::CreateAABB(const Maths::Vector3& point1, const Maths::Vector3& point2)
{
	Maths::AABB aabb;
	aabb.SetMinMax(point1, point2);
	aabb.center.y += 50.0f;
	aabb.extend.y = 100.0f;

	return aabb;
}

void GameApp::SetBlockedCoord(const WOWGE::Maths::Vector3& positionParam)
{
	uint32_t x{ static_cast<uint32_t>(0.5f * positionParam.x / WorldManager::Get()->CellSize()) };
	uint32_t y{ static_cast<uint32_t>(0.5f * positionParam.z / WorldManager::Get()->CellSize()) };

	auto found{ std::find(blockedCoords.begin(), blockedCoords.end(), AI::Graph::Coord{ x, y }) };
	if (found == blockedCoords.end())
	{
		if (x < static_cast<uint32_t>(columns) && y < static_cast<uint32_t>(rows))
		{
			blockedCoords.push_back({ x, y });
			aiWorld.GetNavGraph().GetNode(blockedCoords.back()).blocked = true;
		}
	}
}

void GameApp::SetDestination(const WOWGE::Maths::Vector3& positionParam, std::string name, const WOWGE::Maths::Vector3& normal)
{
	uint32_t x{ static_cast<uint32_t>(0.5f * positionParam.x / WorldManager::Get()->CellSize()) };
	uint32_t y{ static_cast<uint32_t>(0.5f * positionParam.z / WorldManager::Get()->CellSize()) };

	if (x < static_cast<uint32_t>(columns) && y < static_cast<uint32_t>(rows))
	{
		DestinationType destination;
		destination.name = name;
		destination.coordinate = { x, y };
		destination.normal = normal;

		if (WorldManager::Get()->Destinations().size() != 0)
		{
			if (WorldManager::Get()->Destinations().back().name.compare("") != 0)
			{
				WorldManager::Get()->Destinations().emplace_back(destination);
			}
		}
		else
		{
			WorldManager::Get()->Destinations().emplace_back(destination);
		}
	}
}

float GameApp::CalculateCost(AI::Graph::Coord start, AI::Graph::Coord end)
{
	float multiplier;
	if ((start.x != end.x) && (start.y != end.y))
	{
		multiplier = 1.414f;
	}
	else
	{
		multiplier = 1.0f;
	}

	return multiplier;
}

float GameApp::CalculateHeuristic(WOWGE::AI::Graph::Coord start, WOWGE::AI::Graph::Coord end)
{
	float D{ 1.0f };
	float D2{ 1.414f };
	float dx{ static_cast<float>(fabs(static_cast<int>(start.x) - static_cast<int>(end.x))) };
	float dy{ static_cast<float>(fabs(static_cast<int>(start.y) - static_cast<int>(end.y))) };

	return D * (dx + dy) + (D2 - 2.0f * D) * Maths::Min(dx, dy);
}

void GameApp::SaveWalls(const char* fileName)
{
	uint32_t wallCount{ static_cast<uint32_t>(walls.size()) };

	std::ofstream outputFile;
	outputFile.open(fileName, std::ios::out | std::ios::binary);

	outputFile.write((char*)&wallCount, sizeof(uint32_t));

	for (uint32_t i{ 0 }; i < wallCount; ++i)
	{
		outputFile.write((char*)&walls[i], sizeof(Maths::AABB));
	}

	outputFile.close();
}

void GameApp::LoadWalls(const char* fileName)
{
	uint32_t wallCount{ 0 };
	std::ifstream inputFile;
	inputFile.open(fileName, std::ios::in | std::ios::binary);

	inputFile.read((char*)&wallCount, sizeof(uint32_t));

	walls.resize(wallCount);

	for (uint32_t i{ 0 }; i < wallCount; ++i)
	{
		Maths::AABB aabb;
		inputFile.read((char*)&aabb, sizeof(Maths::AABB));

		walls[i] = aabb;
	}

	inputFile.close();
}

void GameApp::SaveBlockedCoords(const char* fileName)
{
	uint32_t blockedCoordsCount{ static_cast<uint32_t>(blockedCoords.size()) };

	std::ofstream outputFile;
	outputFile.open(fileName, std::ios::out | std::ios::binary);

	outputFile.write((char*)&blockedCoordsCount, sizeof(uint32_t));

	for (uint32_t i{ 0 }; i < blockedCoordsCount; ++i)
	{
		outputFile.write((char*)&blockedCoords[i], sizeof(AI::Graph::Coord));
	}

	outputFile.close();
}

void GameApp::LoadBlockedCoords(const char* fileName)
{
	uint32_t blockedCoordsCount{ 0 };
	std::ifstream inputFile;
	inputFile.open(fileName, std::ios::in | std::ios::binary);

	inputFile.read((char*)&blockedCoordsCount, sizeof(uint32_t));

	blockedCoords.resize(blockedCoordsCount);

	for (uint32_t i{ 0 }; i < blockedCoordsCount; ++i)
	{
		AI::Graph::Coord coord;
		inputFile.read((char*)&coord, sizeof(AI::Graph::Coord));

		blockedCoords[i] = coord;
	}

	inputFile.close();
}

void GameApp::SaveDestinations(const char* fileName)
{
	if (WorldManager::Get()->Destinations().back().name.compare("") == 0)
	{
		WorldManager::Get()->Destinations().erase(--WorldManager::Get()->Destinations().end());
	}
	uint32_t destinationsCount{ static_cast<uint32_t>(WorldManager::Get()->Destinations().size()) };

	std::ofstream outputFile;
	outputFile.open(fileName, std::ios::out | std::ios::binary);

	outputFile.write((char*)&destinationsCount, sizeof(uint32_t));

	for (uint32_t i{ 0 }; i < destinationsCount; ++i)
	{
		if (WorldManager::Get()->Destinations()[i].name.compare("") != 0)
		{
			uint32_t stringSize{ static_cast<uint32_t>(WorldManager::Get()->Destinations()[i].name.size()) };
			outputFile.write((char*)&stringSize, sizeof(uint32_t));
			outputFile.write(WorldManager::Get()->Destinations()[i].name.c_str(), stringSize);

			outputFile.write((char*)&WorldManager::Get()->Destinations()[i].coordinate, sizeof(AI::Graph::Coord));

			outputFile.write((char*)&WorldManager::Get()->Destinations()[i].normal, sizeof(Maths::Vector3));
		}
	}

	outputFile.close();
}

void GameApp::LoadDestinations(const char* fileName)
{
	uint32_t destinationsCount{ 0 };
	std::ifstream inputFile;
	inputFile.open(fileName, std::ios::in | std::ios::binary);

	inputFile.read((char*)&destinationsCount, sizeof(uint32_t));

	WorldManager::Get()->Destinations().resize(destinationsCount);

	for (uint32_t i{ 0 }; i < destinationsCount; ++i)
	{
		DestinationType destination;
		uint32_t stringSize{ 0 };
		inputFile.read((char*)&stringSize, sizeof(uint32_t));

		char* temp{ new char[stringSize + 1] };
		inputFile.read(temp, stringSize);
		temp[stringSize] = '\0';
		destination.name = temp;

		inputFile.read((char*)&destination.coordinate, sizeof(AI::Graph::Coord));

		inputFile.read((char*)&destination.normal, sizeof(Maths::Vector3));

		WorldManager::Get()->Destinations()[i] = destination;

		SafeDeleteArray(temp);
	}

	inputFile.close();
}

void GameApp::InitializeOfficeWorkers()
{
	for (uint32_t i{ 0 }; i < officeWorkers.size(); ++i)
	{
		officeWorkers[i] = std::make_unique<OfficeWorker>(aiWorld);
		officeWorkers[i]->SetOffsetMatrix(Maths::Matrix44::Scaling(0.2f, 0.2f, 0.2f) *
			Maths::QuaternionToMatrix(Maths::ToQuaternion({ 0.0f, Maths::ConvertDegreeToRadian(180.0f), 0.0f })));

		officeWorkers[i]->Radius() = 20.0f;

		officeWorkers[i]->GetPathPlanner().SetCostFunc([this](AI::Graph::Coord start, AI::Graph::Coord end)
		{
			return CalculateCost(start, end);
		});

		officeWorkers[i]->GetPathPlanner().SetHeuristicFunc([this](AI::Graph::Coord start, AI::Graph::Coord end)
		{
			return CalculateHeuristic(start, end);
		});

		for (uint32_t j{ 0 }; j < animationNames.size(); ++j)
		{
			officeWorkers[i]->AddAnimation((modelNames[i] + animationNames[j] + ".wowanim").c_str());
		}
	}

	for (uint32_t i{ 0 }; i < officeWorkers.size(); ++i)
	{
		officeWorkers[i]->Initialize((modelNames[i] + ".wow").c_str(), mSceneManager, { 450.0f, 0.0f, 500.0f + (static_cast<float>(i) * 50.0f) }, WorldManager::Get()->MaxAgentSpeed(), 1.0f);
		officeWorkers[i]->GetOfficeWorkerIndex() = i;
	}

	for (uint32_t i{ 0 }; i < officeWorkers.size(); ++i)
	{
		officeWorkers[i]->SetInitialState(StateTypes::Huddle);
	}
}