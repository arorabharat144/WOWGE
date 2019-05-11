#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <AI/Inc/AI.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <Input/Inc/Input.h>

#include <External\ImGui\Inc\imgui.h>

class GameApp : public WOWGE::Core::Application
{
public:
	GameApp();
	~GameApp() override;

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate() override;

	void UpdateSimpleDraw(const WOWGE::Graphics::Camera& camera);
	void UpdateIMGUI(float deltaTime);
	void ClearSelection();

	bool RayCastOnGrid(const WOWGE::Maths::Ray& ray);

	WOWGE::Core::Window mWindow;
	WOWGE::Core::Timer mTimer;

	WOWGE::Graphics::Camera mCamera;

	WOWGE::Graphics::Mesh mCuboid;
	WOWGE::Graphics::MeshBuffer mCuboidMeshBuffer;
	WOWGE::Graphics::Texture mCuboidTexture;

	WOWGE::Graphics::Mesh mPlane;
	WOWGE::Graphics::MeshBuffer mPlaneMeshBuffer;
	WOWGE::Graphics::Texture mPlaneTexture;

	WOWGE::Graphics::VertexShader mShadowMappingVertexShader;
	WOWGE::Graphics::PixelShader mShadowMappingPixelShader;

	//SceneStuff
	WOWGE::Graphics::CameraNode* mCameraNode;
	WOWGE::Graphics::LightNode* mLightNode;
	WOWGE::Graphics::MaterialNode* mMaterialNode;
	WOWGE::Graphics::MeshNode* mCuboidMeshNode;
	WOWGE::Graphics::SamplerNode* mCuboidSamplerNode;
	WOWGE::Graphics::TextureNode* mCuboidTextureNode;
	WOWGE::Graphics::MeshNode* mPlaneMeshNode;
	WOWGE::Graphics::TextureNode* mPlaneTextureNode;
	WOWGE::Graphics::ShaderNode* mShadowMapShaderNode;
	WOWGE::Graphics::SceneManager* mSceneManager;

	std::vector<WOWGE::Graphics::TransformNode*> mCuboidTransformNodes;
	std::vector<WOWGE::Graphics::TransformNode*> mPlaneTransformNodes;

	WOWGE::Graphics::MatrixStack mMatrixStack;

	//AIStuff

	WOWGE::AI::Graph mGraph;

	int mRows{ 20 };
	int mColumns{ 20 };

	int mStartX{ 0 };
	int mStartZ{ 0 };

	int mEndX{ 0 };
	int mEndZ{ 0 };

	void SetBlockedObject(const WOWGE::Maths::Vector3& position);
	void SetWeightedNodes(const WOWGE::Maths::Vector3& position);
	void ConnectBlockedObjectNodes();
	void ConnectWeightedObjectNodes();
	void CopyBlockedObjectsToNewGraph();

	float CalculateCost(WOWGE::AI::Graph::Coord start, WOWGE::AI::Graph::Coord end);
	float CalculateHeuristic(WOWGE::AI::Graph::Coord start, WOWGE::AI::Graph::Coord end);
};

//Homework
//int rows, int columns;
//int startX, int startY;
//int endX, int endY;

#endif // !INCLUDED_GAMEAPP_H
