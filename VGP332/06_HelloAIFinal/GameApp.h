#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core/Inc/Core.h>
#include <AI/Inc/AI.h>
#include <Graphics/Inc/Graphics.h>
#include <Input/Inc/Input.h>

#include <External\ImGui\Inc\imgui.h>

#include <External/DirectXTK/Inc/Audio.h>

#include "OfficeWorker.h"
#include "Destination.h"

class GameApp: public WOWGE::Core::Application
{
public:
	GameApp();
	~GameApp() override;

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate() override;

	void UpdateIMGUI(float deltaTime);
	void UpdateSimpleDraw(WOWGE::Graphics::Camera& camera);

	void AddPointMouseClick(int mouseX, int mouseY, WOWGE::Maths::Vector3& point);
	WOWGE::Maths::AABB CreateAABB(const WOWGE::Maths::Vector3& point1Param, const WOWGE::Maths::Vector3& point2Param);

	void SetBlockedCoord(const WOWGE::Maths::Vector3& positionParam);

	void SetDestination(const WOWGE::Maths::Vector3& positionParam, std::string name, const WOWGE::Maths::Vector3& normal);

	float CalculateCost(WOWGE::AI::Graph::Coord start, WOWGE::AI::Graph::Coord end);
	float CalculateHeuristic(WOWGE::AI::Graph::Coord start, WOWGE::AI::Graph::Coord end);

	void SaveWalls(const char* fileName);
	void LoadWalls(const char* fileName);

	void SaveBlockedCoords(const char* fileName);
	void LoadBlockedCoords(const char* fileName);

	void SaveDestinations(const char* fileName);
	void LoadDestinations(const char* fileName);

	void InitializeOfficeWorkers();

	WOWGE::Core::Window mWindow;
	WOWGE::Core::Timer mTimer;

	WOWGE::Graphics::Mesh mSkySphere;
	WOWGE::Graphics::Texture mSkySphereTexture;
	WOWGE::Graphics::MeshBuffer mSkySphereMeshBuffer;

	WOWGE::Graphics::VertexShader mDepthVertexShader;
	WOWGE::Graphics::PixelShader mDepthPixelShader;

	WOWGE::Graphics::VertexShader mSkySphereVertexShader;
	WOWGE::Graphics::PixelShader mSkySpherePixelShader;

	WOWGE::Graphics::VertexShader mShadowMappingVertexShader;
	WOWGE::Graphics::PixelShader mShadowMappingPixelShader;

	WOWGE::Graphics::Camera mCamera;
	WOWGE::Graphics::Camera mLightCamera;

	WOWGE::Graphics::DepthMap mDepthMap;

	WOWGE::Graphics::RasterizerState mRasterizerState;

	WOWGE::Graphics::RenderTarget* mRenderTarget;

	//SceneStuff
	WOWGE::Graphics::CameraNode* mCameraNode;
	WOWGE::Graphics::LightNode* mLightNode;
	WOWGE::Graphics::MeshNode* mSkySphereMeshNode;
	WOWGE::Graphics::ModelNode* mOfficeModelNode;
	WOWGE::Graphics::RasterizerStateNode* mRasterizerStateNode;
	WOWGE::Graphics::DepthMapNode* mDepthMapNode;
	WOWGE::Graphics::DepthMapNode* mDepthMapNode2ndPass;
	WOWGE::Graphics::SamplerNode* mSamplerNode;
	WOWGE::Graphics::SamplerNode* mDepthSamplerNode;
	WOWGE::Graphics::SamplerNode* mSkySphereSamplerNode;
	WOWGE::Graphics::SceneManager* mSceneManager;
	WOWGE::Graphics::ShaderNode* mDepthMapShaderNode;
	WOWGE::Graphics::ShaderNode* mShadowMapShaderNode;
	WOWGE::Graphics::ShaderNode* mSkySphereShaderNode;
	WOWGE::Graphics::TextureNode* mSkySphereTextureNode;
	WOWGE::Graphics::TransformNode* mSkySphereTransformNode;

	WOWGE::Graphics::MatrixStack mMatrixStack;

	WOWGE::AI::AIWorld aiWorld;
	std::vector<std::unique_ptr<OfficeWorker>> officeWorkers;

	std::vector<std::string> modelNames;
	std::vector<std::string> animationNames;

	uint32_t rows{ 50 };
	uint32_t columns{ 50 };

	std::vector<WOWGE::AI::Graph::Coord> blockedCoords;

	WOWGE::Maths::Plane plane;

	std::unique_ptr<DirectX::AudioEngine> audioEngine;

	std::unique_ptr<DirectX::SoundEffect> officeAmbience;
	std::unique_ptr<DirectX::SoundEffectInstance> officeAmbienceInstance;

	std::unique_ptr<DirectX::SoundEffect> fireAlarm;
	std::unique_ptr<DirectX::SoundEffectInstance> fireAlarmInstance;

};

//Homework
//int rows, int columns;
//int startX, int startY, int startZ;
//int endX, int endY, int endZ;

#endif // !INCLUDED_GAMEAPP_H
