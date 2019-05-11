
#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

using namespace WOWGE;

namespace
{
	float speed = 2.0f;
	float rotationSpeed = 10.0f;

	int a;
	float guiAngle;

	float guiColor[4];
}

GameApp::GameApp():
	mCameraNode(nullptr),
	mLightNode(nullptr),
	mRasterizerStateNode(nullptr),
	mSamplerNode(nullptr),
	mShaderNode(nullptr),
	mModelNode(nullptr),
	mModelNode2(nullptr)
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

	mRasterizerState.Initialize(Graphics::CullMode::None, Graphics::FillMode::Wireframe);

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	mVertexShader.Initialize(L"../../Assets/Shaders/StandardWithDiffuseTexture.fx", "VS", "vs_5_0", Graphics::Vertex::Format);

	///////////////////////////////////////
	// Compile and create the pixel shader

	mPixelShader.Initialize(L"../../Assets/Shaders/StandardWithDiffuseTexture.fx");

	int id = 0;

	mCamera.Initialize(0.785f, (float)w, (float)h, 0.1f, 5000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.SetPosition({ 0.0f, 0.0f, -20.0f });
	mTimer.Initialize();

	mCameraNode = mSceneManager->CreateCameraNode();
	mCameraNode->SetName("Camera");
	mCameraNode->SetCamera(&mCamera);
	mCameraNode->SetCamera(&mCamera, 1);
	 
	mLightNode = mSceneManager->CreateLightNode();
	mLightNode->SetName("Light");
	mLightNode->GetLight().ambient = Maths::Vector4::Black();
	mLightNode->GetLight().diffuse = Maths::Vector4::White();
	mLightNode->GetLight().direction = { -1.0f, -1.0f, 1.0f, 0.0f };
	mLightNode->GetLight().specular = Maths::Vector4::White();

	mSamplerNode = mSceneManager->CreateSamplerNode(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSamplerNode->SetName("Sampler");

	mModelNode = mSceneManager->CreateModelNode("Trex.wow");

	mModelNode2 = mSceneManager->CreateModelNode("StormTrooper.wow");
	mModelNode2->GetTransformNode().GetTransform().SetPosition({ 4.0f, 0.0f, 0.0f });

	mModelNode3 = mSceneManager->CreateModelNode("Naruto.wow");
	mModelNode3->GetTransformNode().GetTransform().SetPosition({ -4.0f, 0.0f, 0.0f });
	mModelNode3->GetTransformNode().GetTransform().SetRotation({ 90.0f, 0.0f, 0.0f });
	mModelNode3->GetTransformNode().GetTransform().SetScale({ 0.01f, 0.01f, 0.01f });

	mModelNode4 = mSceneManager->CreateModelNode("SamsungTV.wow");
	mModelNode4->GetTransformNode().GetTransform().SetPosition({ 0.0f, 0.0f, -10.0f });
	mModelNode4->GetTransformNode().GetTransform().SetRotation({ 90.0f, 0.0f, 0.0f });
	mModelNode4->GetTransformNode().GetTransform().SetScale({ 0.05f, 0.05f, 0.05f });

	mRasterizerStateNode = mSceneManager->CreateRasterizerNode();
	mRasterizerStateNode->SetName("RasterizerState");
	mRasterizerStateNode->SetRasterizerStateNode(&mRasterizerState);
	mRasterizerStateNode->SetEnabled(false);

	mShaderNode = mSceneManager->CreateShaderNode();
	mShaderNode->SetName("Shader");
	mShaderNode->SetPixelShader(&mPixelShader);
	mShaderNode->SetVertexShader(&mVertexShader);

	mSceneManager->GetRoot().AddChild(mCameraNode);
	mCameraNode->AddChild(mRasterizerStateNode);
	mRasterizerStateNode->AddChild(mShaderNode);
	mShaderNode->AddChild(mLightNode);
	mLightNode->AddChild(mSamplerNode);
	mSamplerNode->AddChild(mModelNode);
	mSamplerNode->AddChild(mModelNode2);
	mSamplerNode->AddChild(mModelNode3);
	mSamplerNode->AddChild(mModelNode4);
}

void GameApp::OnTerminate()
{
	mSceneManager->Purge();

	mPixelShader.Terminate();
	mVertexShader.Terminate();

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