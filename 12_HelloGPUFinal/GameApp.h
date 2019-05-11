#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

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

	WOWGE::Core::Window mWindow;
	WOWGE::Core::Timer mTimer;

	WOWGE::Graphics::Mesh mSkySphere;
	WOWGE::Graphics::Texture mSkySphereTexture;
	WOWGE::Graphics::MeshBuffer mSkySphereMeshBuffer;

	WOWGE::Graphics::Mesh mPlane;
	WOWGE::Graphics::Texture mPlaneTexture;
	WOWGE::Graphics::MeshBuffer mPlaneMeshBuffer;

	WOWGE::Graphics::Mesh mPostProcessingPlane;
	WOWGE::Graphics::MeshBuffer mPostProcessingPlaneMeshBuffer;

	WOWGE::Graphics::MeshBuffer mDebugQuadMeshBuffer;

	WOWGE::Graphics::VertexShader mSkySphereVertexShader;
	WOWGE::Graphics::PixelShader mSkySpherePixelShader;

	WOWGE::Graphics::VertexShader mDepthVertexShader;
	WOWGE::Graphics::PixelShader mDepthPixelShader;

	WOWGE::Graphics::VertexShader mShadowMappingVertexShader;
	WOWGE::Graphics::PixelShader mShadowMappingPixelShader;

	WOWGE::Graphics::VertexShader mTVStaticVertexShader;
	WOWGE::Graphics::PixelShader mTVStaticPixelShader;

	WOWGE::Graphics::VertexShader mUIVertexShader;
	WOWGE::Graphics::PixelShader mUIPixelShader;

	WOWGE::Graphics::VertexShader mGroundVertexShader;
	WOWGE::Graphics::PixelShader mGroundPixelShader;

	WOWGE::Graphics::Camera mCamera;
	WOWGE::Graphics::Camera mLightCamera;

	WOWGE::Graphics::DepthMap mDepthMap;

	WOWGE::Graphics::RasterizerState mRasterizerState;

	WOWGE::Graphics::RenderTarget* mRenderTarget;

	struct ConstantBuffer
	{
		WOWGE::Maths::Vector4 values;
	};

	WOWGE::Graphics::AnimationController mMawAnimationController;
	WOWGE::Graphics::AnimationController mMutantAnimationController;

	//SceneStuff
	WOWGE::Graphics::CameraNode* mCameraNode;
	WOWGE::Graphics::ConstantBufferNode<ConstantBuffer>* mConstantBufferNode1;
	WOWGE::Graphics::ConstantBufferNode<ConstantBuffer>* mConstantBufferNode2;
	WOWGE::Graphics::LightNode* mLightNode;
	WOWGE::Graphics::MaterialNode* mPlaneMaterialNode;
	WOWGE::Graphics::MeshNode* mSkySphereMeshNode;
	WOWGE::Graphics::MeshNode* mPlaneMeshNode;
	WOWGE::Graphics::MeshNode* mPostProcessingPlaneMeshNode;
	WOWGE::Graphics::MeshNode* mDebugQuadMeshNode;
	WOWGE::Graphics::ModelNode* mModelNode1;
	WOWGE::Graphics::ModelNode* mModelNode2;
	WOWGE::Graphics::ModelNode* mModelNode3;
	WOWGE::Graphics::ModelNode* mModelNode4;
	WOWGE::Graphics::ModelNode* mModelNode5;
	WOWGE::Graphics::RasterizerStateNode* mRasterizerStateNode;
	WOWGE::Graphics::RenderTargetNode* mRenderTargetNode1;
	WOWGE::Graphics::RenderTargetNode* mRenderTargetNode2;
	WOWGE::Graphics::DepthMapNode* mDepthMapNode;
	WOWGE::Graphics::DepthMapNode* mDepthMapNode2ndPass;
	WOWGE::Graphics::DepthMapNode* mDebugDepthMapNode;
	WOWGE::Graphics::SamplerNode* mSkySphereSamplerNode;
	WOWGE::Graphics::SamplerNode* mSamplerNode;
	WOWGE::Graphics::SamplerNode* mGroundSamplerNode;
	WOWGE::Graphics::SamplerNode* mDepthSamplerNode;
	WOWGE::Graphics::SamplerNode* mUISamplerNode;
	WOWGE::Graphics::SceneManager* mSceneManager;
	WOWGE::Graphics::ShaderNode* mGroundShaderNode;
	WOWGE::Graphics::ShaderNode* mSkySphereShaderNode;
	WOWGE::Graphics::ShaderNode* mDepthMapShaderNode;
	WOWGE::Graphics::ShaderNode* mShadowMapShaderNode;
	WOWGE::Graphics::ShaderNode* mTVStaticShaderNode;
	WOWGE::Graphics::ShaderNode* mUIShaderNode;
	WOWGE::Graphics::TextureNode* mSkySphereTextureNode;
	WOWGE::Graphics::TextureNode* mPlaneTextureNode;
	WOWGE::Graphics::TransformNode* mSkySphereTransformNode;
	WOWGE::Graphics::TransformNode* mPlaneTransformNode;

	WOWGE::Graphics::MatrixStack mMatrixStack;
};

#endif // !INCLUDED_GAMEAPP_H