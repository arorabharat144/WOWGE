#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

using namespace WOWGE;

namespace
{
	const uint32_t w = 1920;
	const uint32_t h = 1080;

	float speed = 50.0f;
	float rotationSpeed = 10.0f;

	Maths::Vector4 lightDirection{ 1.0f, -1.0f, 1.0f, 1.0f };

	int cellSize{ 5 };

	Maths::Vector4 cellColor;

	int startXToDisplay{ 0 };
	int startZToDisplay{ 0 };
	int endXToDisplay{ 20 };
	int endZToDisplay{ 20 };

	int previousStartX{ -1 };
	int previousStartZ{ -1 };
	int previousEndX{ -1 };
	int previousEndZ{ -1 };

	Maths::Vector4 startAgentColor{ Maths::Vector4::Green() };
	Maths::Vector4 endAgentColor{ Maths::Vector4::Red() };

	std::list<AI::Graph::Node> searchResult;
	std::vector<AI::Graph::Coord> path;

	bool ifRunBFS{ false };
	bool ifRunDFS{ false };
	bool ifRunDijkstra{ false };
	bool ifRunAStar{ false };
	bool ifClear{ false };

	bool previousIfRunBFS{ false };
	bool previousIfRunDFS{ false };
	bool previousIfRunDijkstra{ false };
	bool previousIfRunAStar{ false };

	Maths::Ray ray;
	Maths::Vector3 contactPoint;
	uint32_t objectSelector{ 1 };

	//Blocking objects
	std::vector<AI::Graph::Coord> blockedObjects;
	uint32_t previousBlockedObjectSize{ 0 };

	//Weighted graph nodes
	std::vector<AI::Graph::Coord> weightedNodes;
	uint32_t previousWeightedNodesSize{ 0 };
}

GameApp::GameApp() :
	mCameraNode(nullptr),
	mLightNode(nullptr),
	mShadowMapShaderNode(nullptr),
	mSceneManager(nullptr)

{
	mSceneManager = new WOWGE::Graphics::SceneManager(mMatrixStack);
}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), w, h);

	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	Gui::Initialize(mWindow.GetWindowHandle());

	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	Graphics::SimpleDraw::Initialize();

	mShadowMappingVertexShader.Initialize(L"../../Assets/Shaders/StandardWithDiffuseTexture.fx", "VS", "vs_5_0", Graphics::Vertex::Format);
	mShadowMappingPixelShader.Initialize(L"../../Assets/Shaders/StandardWithDiffuseTexture.fx");

	mCamera.Initialize(0.785f, (float)w, (float)h, 1.0f, 5000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.SetPosition({ 60.0f, 100.0f,-100.0f });
	mCamera.Pitch(30.0f);
	mTimer.Initialize();

	mCameraNode = mSceneManager->CreateCameraNode();
	mCameraNode->SetName("Camera");
	mCameraNode->SetCamera(&mCamera, 0);
	mCameraNode->SetCamera(&mCamera, 1); //In case I need a light camera for shadows

	mLightNode = mSceneManager->CreateLightNode();
	mLightNode->SetName("Light");
	mLightNode->GetLight().ambient = Maths::Vector4::Black();
	mLightNode->GetLight().diffuse = Maths::Vector4::White();
	mLightNode->GetLight().direction = { -1.0f, -1.0f, 1.0f, 0.0f };
	mLightNode->GetLight().specular = Maths::Vector4::White();

	mShadowMapShaderNode = mSceneManager->CreateShaderNode();
	mShadowMapShaderNode->SetName("ShadowMapShader");
	mShadowMapShaderNode->SetPixelShader(&mShadowMappingPixelShader);
	mShadowMapShaderNode->SetVertexShader(&mShadowMappingVertexShader);

	Graphics::MeshBuilder::CreateCube(mCuboid);
	mCuboidMeshBuffer.Initialize(mCuboid);

	mCuboidTexture.Initialize("../../Assets/Images/BrickSmallBrown.jpg");

	mCuboidTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mCuboidTextureNode->SetName("CuboidTexture");
	mCuboidTextureNode->SetTexture(&mCuboidTexture);

	mCuboidSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mCuboidSamplerNode->SetName("CuboidSampler");

	mCuboidMeshNode = mSceneManager->CreateMeshNode();
	mCuboidMeshNode->SetName("CuboidMesh");
	mCuboidMeshNode->SetMesh(&mCuboidMeshBuffer);

	mMaterialNode = mSceneManager->CreateMaterialNode();
	mMaterialNode->SetName("CuboidMaterial");
	mMaterialNode->GetMaterial().ambient = Maths::Vector4::Black();
	mMaterialNode->GetMaterial().diffuse = Maths::Vector4::White();
	mMaterialNode->GetMaterial().specular = Maths::Vector4::Black();
	mMaterialNode->GetMaterial().emissive = Maths::Vector4::Black();
	mMaterialNode->GetMaterial().power = 1.0f;

	Graphics::MeshBuilder::CreatePlane(mPlane);
	mPlaneMeshBuffer.Initialize(mPlane);

	mPlaneTexture.Initialize("../../Assets/Images/Water.jpg");

	mPlaneTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	mPlaneTextureNode->SetName("PlaneTexture");
	mPlaneTextureNode->SetTexture(&mPlaneTexture);

	mPlaneMeshNode = mSceneManager->CreateMeshNode();
	mPlaneMeshNode->SetName("PlaneMesh");
	mPlaneMeshNode->SetMesh(&mPlaneMeshBuffer);

	mSceneManager->GetRoot().AddChild(mCameraNode);
	mCameraNode->AddChild(mShadowMapShaderNode);
	mShadowMapShaderNode->AddChild(mLightNode);
	mLightNode->AddChild(mCuboidSamplerNode);
	mCuboidSamplerNode->AddChild(mMaterialNode);

	mMaterialNode->AddChild(mCuboidTextureNode);

	mMaterialNode->AddChild(mPlaneTextureNode);

	mGraph.Init(mColumns, mRows, 5.0f);
}

void GameApp::OnTerminate()
{
	mCameraNode->Terminate();
	mLightNode->Terminate();

	mSceneManager->Purge();

	Graphics::SimpleDraw::Terminate();

	mCuboid.Destroy();
	mCuboidMeshBuffer.Terminate();

	mPlane.Destroy();
	mPlaneMeshBuffer.Terminate();

	mCuboidTexture.Terminate();
	mPlaneTexture.Terminate();

	Gui::Terminate();
	Graphics::GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();
	mWindow.Terminate();

	mGraph.Clear();
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

	auto inputSystem = Input::InputSystem::Get();

	inputSystem->Update();

	if (inputSystem->IsKeyPressed(Input::KeyCode::ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

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

	if (inputSystem->IsKeyPressed(Input::KeyCode::ONE))
	{
		objectSelector = 1;
	}
	if (inputSystem->IsKeyPressed(Input::KeyCode::TWO))
	{
		objectSelector = 2;
	}
	if (inputSystem->IsKeyPressed(Input::KeyCode::THREE))
	{
		objectSelector = 3;
	}
	if (inputSystem->IsKeyPressed(Input::KeyCode::FOUR))
	{
		objectSelector = 4;
	}

	if (inputSystem->IsMouseDown(Input::MouseButton::LBUTTON))
	{
		int mouseX{ inputSystem->GetMouseScreenX() };
		int mouseY{ inputSystem->GetMouseScreenY() };
		ray = mCamera.ScreenPointToRay(mouseX, mouseY, w, h);

		if (RayCastOnGrid(ray) == true)
		{
			if (objectSelector == 1)
			{
				startXToDisplay = static_cast<int>(0.5f * contactPoint.x / static_cast<float>(cellSize));
				startZToDisplay = static_cast<int>(0.5f * contactPoint.z / static_cast<float>(cellSize));
			}
			else if (objectSelector == 2)
			{
				endXToDisplay = static_cast<int>(0.5f * contactPoint.x / static_cast<float>(cellSize));
				endZToDisplay = static_cast<int>(0.5f * contactPoint.z / static_cast<float>(cellSize));
			}
			
		}
	}
	if (inputSystem->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		int mouseX{ inputSystem->GetMouseScreenX() };
		int mouseY{ inputSystem->GetMouseScreenY() };
		ray = mCamera.ScreenPointToRay(mouseX, mouseY, w, h);

		if (RayCastOnGrid(ray) == true)
		{
			if (objectSelector == 3)
			{
				SetBlockedObject(contactPoint);
				ConnectBlockedObjectNodes();
			}
			else if (objectSelector == 4)
			{
				SetWeightedNodes(contactPoint);
				ConnectWeightedObjectNodes();
			}
		}
	}

	mCamera.UpdateViewMatrix();

	// Draw mesh

	mSceneManager->Update(deltaTime);

	if (ifRunBFS)
	{
		if ((previousStartX != startXToDisplay || previousStartZ != startZToDisplay ||
			previousEndX != endXToDisplay || previousEndZ != endZToDisplay) ||
			(previousIfRunDFS == true || previousIfRunDijkstra == true || previousIfRunAStar == true) ||
			previousBlockedObjectSize != blockedObjects.size() ||
			previousWeightedNodesSize != weightedNodes.size())
		{
			ifRunDFS = false;
			ifRunDijkstra = false;
			ifRunAStar = false;

			searchResult.clear();
			path.clear();
			mGraph.Clear();
			mGraph.Init(mColumns, mRows, 5.0f);
			CopyBlockedObjectsToNewGraph();

			if (mGraph.RunBFS({ static_cast<uint32_t>(startXToDisplay), static_cast<uint32_t>(startZToDisplay) }, { static_cast<uint32_t>(endXToDisplay), static_cast<uint32_t>(endZToDisplay) }))
			{
				searchResult = mGraph.GetSearch();
				path = mGraph.GetPath();

				previousStartX = startXToDisplay;
				previousStartZ = startZToDisplay;
				previousEndX = endXToDisplay;
				previousEndZ = endZToDisplay;

				previousIfRunBFS = true;
				previousIfRunDFS = false;
				previousIfRunDijkstra = false;
				previousIfRunAStar = false;

				previousBlockedObjectSize = static_cast<uint32_t>(blockedObjects.size());
				previousWeightedNodesSize = static_cast<uint32_t>(weightedNodes.size());
			}
		}
	}

	if (ifRunDFS)
	{
		if ((previousStartX != startXToDisplay || previousStartZ != startZToDisplay ||
			previousEndX != endXToDisplay || previousEndZ != endZToDisplay) ||
			(previousIfRunBFS == true || previousIfRunDijkstra == true || previousIfRunAStar == true) ||
			previousBlockedObjectSize != blockedObjects.size() ||
			previousWeightedNodesSize != weightedNodes.size())
		{
			ifRunBFS = false;
			ifRunDijkstra = false;
			ifRunAStar = false;

			searchResult.clear();
			path.clear();
			mGraph.Clear();
			mGraph.Init(mColumns, mRows, 5.0f);
			CopyBlockedObjectsToNewGraph();

			if (mGraph.RunDFS({ static_cast<uint32_t>(startXToDisplay), static_cast<uint32_t>(startZToDisplay) }, { static_cast<uint32_t>(endXToDisplay), static_cast<uint32_t>(endZToDisplay) }))
			{
				searchResult = mGraph.GetSearch();
				path = mGraph.GetPath();

				previousStartX = startXToDisplay;
				previousStartZ = startZToDisplay;
				previousEndX = endXToDisplay;
				previousEndZ = endZToDisplay;

				previousIfRunBFS = false;
				previousIfRunDFS = true;
				previousIfRunDijkstra = false;
				previousIfRunAStar = false;

				previousBlockedObjectSize = static_cast<uint32_t>(blockedObjects.size());
				previousWeightedNodesSize = static_cast<uint32_t>(weightedNodes.size());
			}
		}
	}

	if (ifRunDijkstra)
	{
		if ((previousStartX != startXToDisplay || previousStartZ != startZToDisplay ||
			previousEndX != endXToDisplay || previousEndZ != endZToDisplay) || 
			(previousIfRunBFS == true || previousIfRunDFS == true || previousIfRunAStar == true) ||
			previousBlockedObjectSize != blockedObjects.size() ||
			previousWeightedNodesSize != weightedNodes.size())
		{
			ifRunBFS = false;
			ifRunDFS = false;
			ifRunAStar = false;

			searchResult.clear();
			path.clear();
			mGraph.Clear();
			mGraph.Init(mColumns, mRows, 5.0f);
			CopyBlockedObjectsToNewGraph();

			if (mGraph.RunDijkstra({ static_cast<uint32_t>(startXToDisplay), static_cast<uint32_t>(startZToDisplay) },
			{ static_cast<uint32_t>(endXToDisplay), static_cast<uint32_t>(endZToDisplay) },
				[this](AI::Graph::Coord start, AI::Graph::Coord end) { return CalculateCost(start, end); }))
			{
				searchResult = mGraph.GetSearch();
				path = mGraph.GetPath();

				previousStartX = startXToDisplay;
				previousStartZ = startZToDisplay;
				previousEndX = endXToDisplay;
				previousEndZ = endZToDisplay;

				previousIfRunBFS = false;
				previousIfRunDFS = false;
				previousIfRunDijkstra = true;
				previousIfRunAStar = false;

				previousBlockedObjectSize = static_cast<uint32_t>(blockedObjects.size());
				previousWeightedNodesSize = static_cast<uint32_t>(weightedNodes.size());
			}
		}
	}

	if (ifRunAStar)
	{
		if ((previousStartX != startXToDisplay || previousStartZ != startZToDisplay ||
			previousEndX != endXToDisplay || previousEndZ != endZToDisplay) ||
			(previousIfRunBFS == true || previousIfRunDFS == true || previousIfRunDijkstra == true) ||
			previousBlockedObjectSize != blockedObjects.size() ||
			previousWeightedNodesSize != weightedNodes.size())
		{
			ifRunBFS = false;
			ifRunDFS = false;
			ifRunDijkstra = false;

			searchResult.clear();
			path.clear();
			mGraph.Clear();
			mGraph.Init(mColumns, mRows, 5.0f);
			CopyBlockedObjectsToNewGraph();

			if (mGraph.RunAStar({ static_cast<uint32_t>(startXToDisplay), static_cast<uint32_t>(startZToDisplay) },
			{ static_cast<uint32_t>(endXToDisplay), static_cast<uint32_t>(endZToDisplay) },
				[this](AI::Graph::Coord start, AI::Graph::Coord end) { return CalculateCost(start, end); },
				[this](AI::Graph::Coord start, AI::Graph::Coord end) { return CalculateHeuristic(start, end); }))
			{
				searchResult = mGraph.GetSearch();
				path = mGraph.GetPath();

				previousStartX = startXToDisplay;
				previousStartZ = startZToDisplay;
				previousEndX = endXToDisplay;
				previousEndZ = endZToDisplay;

				previousIfRunBFS = false;
				previousIfRunDFS = false;
				previousIfRunDijkstra = false;
				previousIfRunAStar = true;

				previousBlockedObjectSize = static_cast<uint32_t>(blockedObjects.size());
				previousWeightedNodesSize = static_cast<uint32_t>(weightedNodes.size());
			}
		}
	}

	if (ifClear)
	{
		searchResult.clear();
		path.clear();
		mGraph.Clear();
		mGraph.Init(mColumns, mRows, 5.0f);
		CopyBlockedObjectsToNewGraph();
	}

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();
	mSceneManager->Render();

	UpdateSimpleDraw(mCamera);

	UpdateIMGUI(deltaTime);

	graphicsSystem->EndRender();
}

void GameApp::UpdateSimpleDraw(const WOWGE::Graphics::Camera& camera)
{
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			float centreX = 2 * x * static_cast<float>(cellSize);
			float centreZ = 2 * y * static_cast<float>(cellSize);

			Graphics::SimpleDraw::AddSquare({ centreX, 0.0f, centreZ }, static_cast<float>(cellSize), cellColor, Graphics::SimpleDraw::Axis::YAxis);
		}
	}

	float agentSize = 2.0f;
	Graphics::SimpleDraw::AddSphere({ static_cast<float>(mStartX), 0.0f, static_cast<float>(mStartZ) }, agentSize, startAgentColor, 10, 10);
	Graphics::SimpleDraw::AddSphere({ static_cast<float>(mEndX), 0.0f, static_cast<float>(mEndZ) }, agentSize, endAgentColor, 10, 10);

	for (uint32_t i = 0; i < path.size(); ++i)
	{
		if (i == path.size() - 1)
		{
			break;
		}

		float pointX = 2 * path[i].x * static_cast<float>(cellSize);
		float pointZ = 2 * path[i].y * static_cast<float>(cellSize);

		float nextpointX = 2 * path[i + 1].x * static_cast<float>(cellSize);
		float nextpointZ = 2 * path[i + 1].y * static_cast<float>(cellSize);

		Graphics::SimpleDraw::AddLine({ pointX, 0.0f, pointZ }, { nextpointX, 0.0f, nextpointZ }, Maths::Vector4::Blue());
	}

	for (auto it = searchResult.begin(); it != searchResult.end(); ++it)
	{
		if ((*it).parent != nullptr)
		{
			float pointX = 2 * (*it).coord.x * static_cast<float>(cellSize);
			float pointZ = 2 * (*it).coord.y * static_cast<float>(cellSize);

			float nextpointX = 2 * (*it).parent->coord.x * static_cast<float>(cellSize);
			float nextpointZ = 2 * (*it).parent->coord.y * static_cast<float>(cellSize);

			Graphics::SimpleDraw::AddLine({ pointX, 0.0f, pointZ }, { nextpointX, 0.0f, nextpointZ }, Maths::Vector4::Red());
		}
	}

	Graphics::SimpleDraw::Render(camera);
}

void GameApp::UpdateIMGUI(float deltaTime)
{
	Gui::BeginRender(deltaTime);

	ImGui::SetNextWindowSize({ 300.0f, 400.0f });
	ImGui::Begin("Grid Maker");
	ImGui::SetWindowPos({ 0.0f, 600.0f });

	ImGui::Text("Grid\n");
	if (ImGui::InputInt("Rows", &mRows, 1, 100, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
	{
		searchResult.clear();
		path.clear();
		mGraph.Clear();
		mGraph.Init(mColumns, mRows, 5.0f);
		CopyBlockedObjectsToNewGraph();
	}
	if (ImGui::InputInt("Columns", &mColumns, 1, 100, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
	{
		searchResult.clear();
		path.clear();
		mGraph.Clear();
		mGraph.Init(mColumns, mRows, 5.0f);
		CopyBlockedObjectsToNewGraph();
	}
	//ImGui::SliderFloat("Cell Size", &cellSize, 0.0f, 10.0f);
	ImGui::Text("\n");

	float cellColorFloat[3] = { cellColor.x, cellColor.y, cellColor.z };
	ImGui::ColorPicker3("Cell Color", cellColorFloat);
	cellColor = { cellColorFloat[0], cellColorFloat[1], cellColorFloat[2], 1.0f };

	ImGui::End();

	ImGui::SetNextWindowSize({ 600.0f, 400.0f });
	ImGui::Begin("Agents");
	ImGui::SetWindowPos({ 1300.0f, 600.0f });

	ImGui::PushItemWidth(200.0f);
	
	ImGui::InputInt("Start PosX", &startXToDisplay); ImGui::SameLine(300.0f);
	startXToDisplay = std::min(startXToDisplay, mColumns - 1);
	startXToDisplay = std::max(startXToDisplay, 0);
	
	ImGui::InputInt("End PosX", &endXToDisplay);
	endXToDisplay = std::min(endXToDisplay, mColumns - 1);
	endXToDisplay = std::max(endXToDisplay, 0);
	
	ImGui::InputInt("Start PosZ", &startZToDisplay); ImGui::SameLine(300.0f);
	startZToDisplay = std::min(startZToDisplay, mRows - 1);
	startZToDisplay = std::max(startZToDisplay, 0);
	
	ImGui::InputInt("End PosZ\n", &endZToDisplay);
	endZToDisplay = std::min(endZToDisplay, mRows - 1);
	endZToDisplay = std::max(endZToDisplay, 0);
	
	mStartX = 2 * startXToDisplay * cellSize;
	mEndX = 2 * endXToDisplay * cellSize;
	mStartZ = 2 * startZToDisplay * cellSize;
	mEndZ = 2 * endZToDisplay * cellSize;

	//ImGui::Text("\n");

	float startAgentColorFloat[3]{ startAgentColor.x, startAgentColor.y, startAgentColor.z };
	float endAgentColorFloat[3]{ endAgentColor.x, endAgentColor.y, endAgentColor.z };
	ImGui::ColorPicker3("Start Color", startAgentColorFloat); ImGui::SameLine(300.0f);
	ImGui::ColorPicker3("End Color", endAgentColorFloat);
	startAgentColor = { startAgentColorFloat[0], startAgentColorFloat[1], startAgentColorFloat[2], 1.0f };
	endAgentColor = { endAgentColorFloat[0], endAgentColorFloat[1], endAgentColorFloat[2], 1.0f };

	ImGui::Text("Press 1 to select the start agent, 2 to select the end agent,\n 3 to get the blocking object selected,\n 4 to get the weighted object selected");


	if (ImGui::Button("Run BFS"))
	{
		ClearSelection();
		ifRunBFS = true;
	}
	ImGui::SameLine();

	if (ImGui::Button("Run DFS"))
	{
		ClearSelection();
		ifRunDFS = true;
	}
	ImGui::SameLine();

	if (ImGui::Button("Run Dijkstra"))
	{
		ClearSelection();
		ifRunDijkstra = true;
	}
	ImGui::SameLine();

	if (ImGui::Button("Run A*"))
	{
		ClearSelection();
		ifRunAStar = true; 
	}
	ImGui::SameLine();

	ifClear = ImGui::Button("Clear Search");

	ImGui::End();

	Gui::EndRender();
}

void GameApp::ClearSelection()
{
	ifRunBFS = false;
	ifRunDFS = false;
	ifRunDijkstra = false;
	ifRunAStar = false;
}

bool GameApp::RayCastOnGrid(const WOWGE::Maths::Ray& ray)
{
	if (Maths::Intersect(ray, { 0.0f - static_cast<float>(cellSize), 0.0f, 0.0f },
		{ 0.0f - static_cast<float>(cellSize), 0.0f, 2 * (mRows) * static_cast<float>(cellSize) },
		{ 2 * (mColumns) * static_cast<float>(cellSize), 0.0f, 0.0f }, contactPoint) == true ||
		Maths::Intersect(ray, { 0.0f - static_cast<float>(cellSize), 0.0f, 2 * (mRows) * static_cast<float>(cellSize) },
		{ 2 * (mColumns) * static_cast<float>(cellSize), 0.0f, 2 * (mRows) * static_cast<float>(cellSize) },
		{ 2 * (mColumns) * static_cast<float>(cellSize), 0.0f, 0.0f }, contactPoint) == true)
	{
		return true;
	}
	return false;
}

void GameApp::SetBlockedObject(const WOWGE::Maths::Vector3& position)
{
	uint32_t x{ static_cast<uint32_t>(0.5f * position.x / static_cast<float>(cellSize)) };
	uint32_t y{ static_cast<uint32_t>(0.5f * position.z / static_cast<float>(cellSize)) };

	if (x < static_cast<uint32_t>(mColumns) && y < static_cast<uint32_t>(mRows))
	{
		blockedObjects.push_back({ x,y });
		mGraph.GetNode(blockedObjects.back()).blocked = true;
	}
}

void GameApp::SetWeightedNodes(const WOWGE::Maths::Vector3& position)
{
	weightedNodes.push_back(
	{
		static_cast<uint32_t>(0.5f * position.x / static_cast<float>(cellSize)),
		static_cast<uint32_t>(0.5f * position.z / static_cast<float>(cellSize))
	});
}

void GameApp::ConnectBlockedObjectNodes()
{
	//connect texture to the transform
	//connect transform to the mesh
	Graphics::TransformNode* transformNode;
	transformNode = mSceneManager->CreateTransformNode();
	transformNode->SetName("CuboidTransform");

	float scaleY{ 20.0f };

	float renderPositionX{ static_cast<float>(2 * blockedObjects.back().x * cellSize) };
	float renderPositionZ{ static_cast<float>(2 * blockedObjects.back().y * cellSize) };

	transformNode->GetTransform().SetPosition({ renderPositionX, scaleY * 0.5f, renderPositionZ });
	transformNode->GetTransform().SetScale({ static_cast<float>(cellSize), scaleY, static_cast<float>(cellSize) });

	mCuboidTransformNodes.push_back(transformNode);

	mCuboidTextureNode->AddChild(mCuboidTransformNodes.back());
	mCuboidTransformNodes.back()->AddChild(mCuboidMeshNode);
}

void GameApp::ConnectWeightedObjectNodes()
{
	//connect texture to the transform
	//connect transform to the mesh
	Graphics::TransformNode* transformNode;
	transformNode = mSceneManager->CreateTransformNode();
	transformNode->SetName("CuboidTransform");

	float renderPositionX{ static_cast<float>(2 * weightedNodes.back().x * cellSize) };
	float renderPositionZ{ static_cast<float>(2 * weightedNodes.back().y * cellSize) };

	transformNode->GetTransform().SetPosition({ renderPositionX - static_cast<float>(cellSize), 0.0f, renderPositionZ - static_cast<float>(cellSize) });
	transformNode->GetTransform().SetRotation({ 90.0f, 0.0f, 0.0f });
	transformNode->GetTransform().SetScale({ static_cast<float>(2 * cellSize), static_cast<float>(2 * cellSize), 1.0f });

	mPlaneTransformNodes.push_back(transformNode);

	mPlaneTextureNode->AddChild(mPlaneTransformNodes.back());
	mPlaneTransformNodes.back()->AddChild(mPlaneMeshNode);
}

void GameApp::CopyBlockedObjectsToNewGraph()
{
	for (uint32_t i{ 0 }; i < blockedObjects.size(); ++i)
	{
		mGraph.GetNode(blockedObjects[i]).blocked = true;
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

	float cost{ 1.0f };
	for (uint32_t i{ 0 }; i < weightedNodes.size(); ++i)
	{
		if (start == weightedNodes[i] || end == weightedNodes[i])
		{
			cost = 10.0f;
			break;
		}
	}

	return multiplier * cost;
}

float GameApp::CalculateHeuristic(WOWGE::AI::Graph::Coord start, WOWGE::AI::Graph::Coord end)
{
	float D{ 1.0f };
	float D2{ 1.414f };
	float dx{ static_cast<float>(fabs(static_cast<int>(start.x) - static_cast<int>(end.x))) };
	float dy{ static_cast<float>(fabs(static_cast<int>(start.y) - static_cast<int>(end.y))) };

	return D * (dx + dy) + (D2 - 2.0f * D) * Maths::Min(dx, dy);
}