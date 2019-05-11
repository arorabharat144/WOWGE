#include "GameApp.h"

#include <Graphics\Inc\Gui.h>

using namespace WOWGE;

namespace
{
	float speed{ 50.0f };
	float rotationSpeed{ 10.0f };

	float targetSpeed{ 50.0f };

	uint32_t numberOfBones{ 50 };
	Maths::Vector4 lightDirection{ 1.0f, -1.0f, 1.0f, 1.0f };

	Maths::Matrix44 initialTransform;

	float cylinderHeight{ 5.0f };

	void RunBackward(std::vector<Maths::Matrix44>& boneTransforms, const Maths::Vector3& target, float boneDistance)
	{
		Maths::Vector3 position{ target };

		for (uint32_t i{ static_cast<uint32_t>(boneTransforms.size() - 1) }; i > 0; --i)
		{
			Maths::Vector3 direction{ position - Maths::GetTranslation(boneTransforms[i]) };
			float lengthBetweenBones{ boneDistance / Maths::Magnitude(direction) };
			Maths::Vector3 newPosition{ position * (1.0f - lengthBetweenBones) + Maths::GetTranslation(boneTransforms[i]) * lengthBetweenBones };
			boneTransforms[i] = Maths::Matrix44::Translation(newPosition);
			position = Maths::GetTranslation(boneTransforms[i]);
		}

		//for (uint32_t i{ static_cast<uint32_t>(boneTransforms.size() - 1) }; i > 0; --i)
		//{
		//	Maths::Vector3 direction;
		//	if (i == static_cast<uint32_t>(boneTransforms.size() - 1))
		//	{
		//		direction = Maths::Normalize(target - Maths::GetTranslation(boneTransforms[i]));
		//	}
		//	else
		//	{
		//		direction = Maths::Normalize(Maths::GetTranslation(boneTransforms[i + 1]) - Maths::GetTranslation(boneTransforms[i]));
		//	}
		//
		//	float length{ Maths::Distance(Maths::GetTranslation(boneTransforms[i]), Maths::GetTranslation(boneTransforms[i - 1])) };
		//
		//	boneTransforms[i] = Maths::Matrix44::Translation(direction * length);
		//}
	}

	void RunForward(std::vector<Maths::Matrix44>& boneTransforms, const Maths::Vector3& target, float boneDistance)
	{
		Maths::Vector3 position{ Maths::GetTranslation(boneTransforms[1]) };

		for (uint32_t i{ 0 }; i < boneTransforms.size() - 1; ++i)
		{
			Maths::Vector3 direction{ position - Maths::GetTranslation(boneTransforms[i]) };

			float lengthBetweenBones{ boneDistance / Maths::Magnitude(direction) };

			Maths::Vector3 newPosition{ Maths::GetTranslation(boneTransforms[i]) * (1.0f - lengthBetweenBones) + position * lengthBetweenBones };

			boneTransforms[i + 1] = Maths::Matrix44::Translation(newPosition);

			if (i == boneTransforms.size() - 2)
			{
				position = target;
			}
			else
			{
				position = Maths::GetTranslation(boneTransforms[i + 2]);
			}
		}

		//for (uint32_t i{ 0 }; i < boneTransforms.size(); ++i)
		//{
		//	Maths::Vector3 direction;
		//	float length{ 0.0f };
		//	if (i == boneTransforms.size() - 1)
		//	{
		//		direction = Maths::Normalize(target - Maths::GetTranslation(boneTransforms[i]));
		//		length = Maths::Magnitude(Maths::GetTranslation(boneTransforms[i]) - Maths::GetTranslation(boneTransforms[i - 1]));
		//		boneTransforms[i] = Maths::Matrix44::Translation(direction * length);
		//
		//		return;
		//	}
		//
		//	direction = Maths::Normalize(Maths::GetTranslation(boneTransforms[i + 1]) - Maths::GetTranslation(boneTransforms[i]));
		//	length = Maths::Magnitude(Maths::GetTranslation(boneTransforms[i]) - Maths::GetTranslation(boneTransforms[i + 1]));
		//
		//	boneTransforms[i] = Maths::Matrix44::Translation(direction * length);
		//}
	}

	void RunFABRIK(std::vector<Maths::Matrix44>& boneTransforms, const Maths::Vector3& target, float boneDistance, uint32_t iterations, float tolerance)
	{
		float distance{ Maths::Magnitude(Maths::GetTranslation(boneTransforms[0]) - target) };

		float totalDistance{ boneDistance * (boneTransforms.size()) };

		//if (distance > totalDistance)
		//{
		//	for (uint32_t i{ 0 }; i < boneTransforms.size() - 1; ++i)
		//	{
		//		float r{ Maths::Magnitude(target - Maths::GetTranslation(boneTransforms[i])) };
		//		float length{ boneDistance / r };
		//
		//		boneTransforms[i + 1] = Maths::Matrix44::Translation(Maths::GetTranslation(boneTransforms[i]) * (1.0f - length) + target * length);
		//	}
		//}
		//else
		{
			uint32_t currentIteration{ 0 };

			float differential{ Maths::Magnitude(Maths::GetTranslation(boneTransforms.back()) - target) };

			while (differential > tolerance)
			{
				RunBackward(boneTransforms, target, boneDistance);
				RunForward(boneTransforms, target, boneDistance);

				differential = Maths::Magnitude(Maths::GetTranslation(boneTransforms.back()) - target);

				currentIteration++;

				if (currentIteration > iterations)
				{
					break;
				}
			}
		}
	}
}

GameApp::GameApp() :
	mCameraNode(nullptr),
	mLightNode(nullptr),
	//mPlaneMaterialNode(nullptr),
	//mPlaneMeshNode(nullptr),
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
	//mPlaneTextureNode(nullptr),
	mSkySphereTextureNode(nullptr),
	//mPlaneTransformNode(nullptr),
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

	//Graphics::SkinnedMeshBuilder::CreatePlane(mPlane);
	//mPlaneMeshBuffer.Initialize(mPlane);
	//mPlaneTexture.Initialize("../../Assets/Images/BrickSmallBrown.jpg");

	mRasterizerState.Initialize(Graphics::CullMode::None, Graphics::FillMode::Wireframe);

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

	//Tentacle Arm

	//Graphics::SkinnedMeshBuilder::CreateCylinder(mTentacleArmMesh, 200, 200, cylinderHeight, 5.0f);
	//mTentacleArmMeshBuffer.Initialize(mTentacleArmMesh);


	//mTentacleArmTexture.Initialize("../../Assets/Images/Metal.jpg");

	mCamera.Initialize(0.785f, (float)w, (float)h, 1.0f, 5000.0f);
	mCamera.InitializePerspectiveProjectionMatrix();
	mCamera.SetPosition({ 0.0f, 20.0f,-100.0f });
	mTimer.Initialize();

	mLightCamera.Initialize(0.785f, (float)w, (float)h, 25.0f, 500.0f);
	mLightCamera.InitializeOrthographicProjectionMatrix(widthLightCamera, heightLightCamera);
	mLightCamera.Pitch(30.0f);
	mLightCamera.SetPosition({ 0.0f, 50.0f,-40.0f });

	mCameraNode = mSceneManager->CreateCameraNode();
	mCameraNode->SetName("Camera");
	mCameraNode->SetCamera(&mCamera, 0);
	mCameraNode->SetCamera(&mLightCamera, 1);

	mBoneWorldTransformsNode = mSceneManager->CreateConstantBufferNode<ConstantBuffer>(5);
	mBoneWorldTransformsNode->SetName("BoneWorldTransformsNode");

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

	//mPlaneTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	//mPlaneTextureNode->SetName("PlaneTexture");
	//mPlaneTextureNode->SetTexture(&mPlaneTexture);
	//mHasTexturesNode[0]->GetData().hasTextures.x = 1.0f;

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

	//mPlaneTransformNode = mSceneManager->CreateTransformNode();
	//mPlaneTransformNode->SetName("Transform");
	//mPlaneTransformNode->GetTransform().SetPosition({ -200.0f, 0.0f, -150.0f });
	//mPlaneTransformNode->GetTransform().SetScale({ 500.0f, 500.0f, 1.0f });
	//mPlaneTransformNode->GetTransform().SetRotation({ 90.0f, 0.0f, 0.0f });

	mSkySphereMeshNode = mSceneManager->CreateMeshNode();
	mSkySphereMeshNode->SetName("SkySphereMesh");
	mSkySphereMeshNode->SetMesh(&mSkySphereMeshBuffer);

	//mPlaneMeshNode = mSceneManager->CreateMeshNode();
	//mPlaneMeshNode->SetMesh(&mPlaneMeshBuffer);

	//mPlaneMaterialNode = mSceneManager->CreateMaterialNode();
	//mPlaneMaterialNode->GetMaterial().ambient = Maths::Vector4::Black();
	//mPlaneMaterialNode->GetMaterial().diffuse = Maths::Vector4::White();
	//mPlaneMaterialNode->GetMaterial().specular = Maths::Vector4::Gray();
	//mPlaneMaterialNode->GetMaterial().emissive = Maths::Vector4::Black();
	//mPlaneMaterialNode->GetMaterial().power = 1.0f;

	//Initializing Tentacle Arm Nodes

	//mTentacleMaterialNode = mSceneManager->CreateMaterialNode();
	//mTentacleMaterialNode->GetMaterial().ambient = Maths::Vector4::Black();
	//mTentacleMaterialNode->GetMaterial().diffuse = Maths::Vector4::White();
	//mTentacleMaterialNode->GetMaterial().specular = Maths::Vector4::Gray();
	//mTentacleMaterialNode->GetMaterial().emissive = Maths::Vector4::Black();
	//mTentacleMaterialNode->GetMaterial().power = 1.0f;

	//mTentacleArmMeshNode = mSceneManager->CreateMeshNode();
	//mTentacleArmMeshNode->SetName("TentacleArmMesh");
	//mTentacleArmMeshNode->SetMesh(&mTentacleArmMeshBuffer);

	//mTentacleArmTransformNode = mSceneManager->CreateTransformNode();
	//mTentacleArmTransformNode->SetName("TentacleArmTransform");

	//mTentacleArmTransformNode[i]->GetTransform().SetPivot({ mTentacleArmTransformNode[0]->GetTransform().GetScale().x * (-0.5f), 0.0f, 0.0f });

	//mTentacleArmTransformNode->GetTransform().SetPosition({ 0.0f, 5.0f, 0.0f });
	//initialTransform = mTentacleArmTransformNode->GetTransform().GetTransformMatrix();

	//mTentacleArmTextureNode = mSceneManager->CreateTextureNode(ShaderStage::PixelShader);
	//mTentacleArmTextureNode->SetName("TentacleArmTexture");
	//mTentacleArmTextureNode->SetTexture(&mTentacleArmTexture);
	//mTentacleArmTextureNode->SetIndex(0);


	mSceneManager->GetRoot().AddChild(mCameraNode);

	mCameraNode->AddChild(mDepthMapNode);
	mDepthMapNode->AddChild(mDepthMapShaderNode);

	mDepthMapShaderNode->AddChild(mBoneWorldTransformsNode);
	//mBoneWorldTransformsNode->AddChild(mTentacleArmTransformNode);
	//mTentacleArmTransformNode->AddChild(mHasTexturesNode[0]);
	//mHasTexturesNode[0]->AddChild(mTentacleArmMeshNode);

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

	//mSamplerNode->AddChild(mPlaneTransformNode);
	//mPlaneTransformNode->AddChild(mPlaneTextureNode);
	//mPlaneTextureNode->AddChild(mPlaneMaterialNode);
	//mPlaneMaterialNode->AddChild(mPlaneMeshNode);

	//mSamplerNode->AddChild(mTentacleMaterialNode);
	//mTentacleMaterialNode->AddChild(mHasTexturesNode[0]);
	//mTentacleMaterialNode->AddChild(mTentacleArmTextureNode);

	//mTentacleArmTextureNode->AddChild(mBoneWorldTransformsNode);
	//mBoneWorldTransformsNode->AddChild(mTentacleArmTransformNode);

	GameApp::InitializeBones();

	Graphics::SimpleDraw::Initialize();
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

	//mPlaneTexture.Terminate();
	//mTentacleArmTexture.Terminate();
	mSkySphereTexture.Terminate();
	//mPlane.Destroy();
	mSkySphere.Destroy();

	//mTentacleArmMesh.Destroy();
	//mTentacleArmMeshBuffer.Terminate();

	//mPlaneMeshBuffer.Terminate();

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
		if (inputSystem->IsKeyDown(Input::KeyCode::A))
		{
			mTarget.x -= targetSpeed * deltaTime;
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::D))
		{
			mTarget.x += targetSpeed * deltaTime;
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::S))
		{
			mTarget.y -= targetSpeed * deltaTime;
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::W))
		{
			mTarget.y += targetSpeed * deltaTime;
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::Q))
		{
			mTarget.z += targetSpeed * deltaTime;
		}
		if (inputSystem->IsKeyDown(Input::KeyCode::E))
		{
			mTarget.z -= targetSpeed * deltaTime;
		}
	}
	if (inputSystem->IsKeyPressed(Input::KeyCode::F1))
	{
		mRasterizerStateNode->SetEnabled(!mRasterizerStateNode->GetEnabled());
	}

	mSkySphereTransformNode->GetTransform().SetPosition(mCamera.GetPosition());

	//mTentacleArmTransformNode->GetTransform().SetTransformMatrix(initialTransform * mBoneWorldTransform[i]); //Fixing the bone transform with a correction matrix(initialTransform) to render boxes, as the Tentacle arm

	for (uint32_t i{ 0 }; i < numberOfBones; ++i)
	{
		mBoneWorldTransformsNode->GetData().boneTransforms[i] = Maths::Transpose(mBones[i].offsetTransform * mBoneWorldTransform[i]);
	}

	UpdateBones();

	RunFABRIK(mBoneWorldTransform, mTarget, cylinderHeight, 10, 0.1f);

	mCamera.UpdateViewMatrix();
	mLightCamera.UpdateViewMatrix();

	// Draw mesh

	mSceneManager->Update(deltaTime);

	auto graphicsSystem = Graphics::GraphicsSystem::Get();
	graphicsSystem->BeginRender();
	mSceneManager->Render();

	UpdateSimpleDraw(mCamera);

	graphicsSystem->EndRender();
}

void GameApp::InitializeBones()
{
	//Code for skeleton framework
	mBones.resize(numberOfBones);
	mBoneWorldTransform.resize(numberOfBones);

	for (uint32_t i{ 0 }; i < numberOfBones; ++i)
	{
		mBones[i].index = i;
	
	}

	for (uint32_t i = 1; i < numberOfBones; ++i)
	{
		mBones[i].parent = &mBones[i - 1];
		mBones[i].transform = Maths::Matrix44::Translation(0.0f, cylinderHeight, 0.0f);
	}
}

void GameApp::UpdateBones()
{
	for (uint32_t i{ 0 }; i < numberOfBones; ++i)
	{
		Graphics::Bone* bone{ &mBones[i] };
		mBoneWorldTransform[i] = bone->transform;
		while (bone->parent)
		{
			mBoneWorldTransform[i] = mBoneWorldTransform[i] * bone->parent->transform;

			bone = bone->parent;
		}
	}
}

void GameApp::UpdateSimpleDraw(WOWGE::Graphics::Camera& camera)
{
	Graphics::SimpleDraw::AddSphere(mTarget, 5.0f, Maths::Vector4::Magenta());

	for (uint32_t i{ 0 }; i < mBones.size(); ++i)
	{
		Graphics::SimpleDraw::AddSphere(Maths::GetTranslation(mBoneWorldTransform[i]), 1.0f, Maths::Vector4::Red(), 10, 10);

		Graphics::Bone* bone{ &mBones[i] };
		while (bone->parent)
		{
			Graphics::SimpleDraw::AddLine(Maths::GetTranslation(mBoneWorldTransform[bone->index]), Maths::GetTranslation(mBoneWorldTransform[bone->parent->index]), Maths::Vector4::Blue());
			bone = bone->parent;
		}
	}

	Graphics::SimpleDraw::Render(camera);
}