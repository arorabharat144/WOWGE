#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <AI/Inc/AI.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <Input/Inc/Input.h>

#include <External/ImGui/Inc/imgui.h>

#include "Cylinder.h"

class GameApp : public WOWGE::Core::Application
{
public:
	GameApp();
	~GameApp() override;

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate() override;

	void UpdateIMGUI(float deltaTime);
	void UpdateSimpleDraw(const WOWGE::Graphics::Camera& camera);

	WOWGE::Core::Window mWindow;
	WOWGE::Core::Timer mTimer;

	WOWGE::Graphics::Mesh mSkySphere;
	WOWGE::Graphics::Texture mSkySphereTexture;
	WOWGE::Graphics::MeshBuffer mSkySphereMeshBuffer;

	WOWGE::Graphics::Mesh mPlane;
	WOWGE::Graphics::Texture mPlaneTexture;
	WOWGE::Graphics::MeshBuffer mPlaneMeshBuffer;

	WOWGE::Graphics::Mesh mCylinder;
	WOWGE::Graphics::MeshBuffer mCylinderMeshBuffer;
	WOWGE::Graphics::MeshNode* mCylinderMeshNode;
	std::vector<WOWGE::Graphics::MaterialNode*> mCylinderMaterialsNodes;

	std::vector<WOWGE::Graphics::TransformNode*> mCylinderTransformsNodes;

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

	struct HasTextureBuffer
	{
		WOWGE::Maths::Vector4 hasTextures;
	};

	std::vector<WOWGE::Graphics::ConstantBufferNode<HasTextureBuffer>*> mHasTexturesNode;

	//SceneStuff
	WOWGE::Graphics::CameraNode* mCameraNode;
	WOWGE::Graphics::LightNode* mLightNode;
	WOWGE::Graphics::MaterialNode* mPlaneMaterialNode;
	WOWGE::Graphics::MeshNode* mPlaneMeshNode;
	WOWGE::Graphics::ModelNode* mModelNode;
	WOWGE::Graphics::MeshNode* mSkySphereMeshNode;
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
	WOWGE::Graphics::TextureNode* mPlaneTextureNode;
	WOWGE::Graphics::TextureNode* mSkySphereTextureNode;
	WOWGE::Graphics::TransformNode* mPlaneTransformNode;
	WOWGE::Graphics::TransformNode* mSkySphereTransformNode;

	WOWGE::Graphics::MatrixStack mMatrixStack;

	WOWGE::AI::AIWorld mAIWorld;
	std::vector<std::unique_ptr<Cylinder>> mEntities;
};

#endif // !INCLUDED_GAMEAPP_H
