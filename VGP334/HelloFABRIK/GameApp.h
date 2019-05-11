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
	void InitializeBones();
	void UpdateBones();

	void UpdateSimpleDraw(WOWGE::Graphics::Camera& camera);

	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate() override;

	WOWGE::Core::Window mWindow;
	WOWGE::Core::Timer mTimer;

	//WOWGE::Graphics::SkinnedMesh mTentacleArmMesh;
	//WOWGE::Graphics::MeshBuffer mTentacleArmMeshBuffer;
	//WOWGE::Graphics::Texture mTentacleArmTexture;

	WOWGE::Graphics::Mesh mSkySphere;
	WOWGE::Graphics::Texture mSkySphereTexture;
	WOWGE::Graphics::MeshBuffer mSkySphereMeshBuffer;

	//WOWGE::Graphics::SkinnedMesh mPlane;
	//WOWGE::Graphics::Texture mPlaneTexture;
	//WOWGE::Graphics::MeshBuffer mPlaneMeshBuffer;

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

	struct ConstantBuffer
	{
		WOWGE::Maths::Matrix44 boneTransforms[200];
	};

	//SceneStuff
	WOWGE::Graphics::CameraNode* mCameraNode;
	WOWGE::Graphics::ConstantBufferNode<ConstantBuffer>* mBoneWorldTransformsNode;
	WOWGE::Graphics::LightNode* mLightNode;
	//WOWGE::Graphics::MaterialNode* mPlaneMaterialNode;
	//WOWGE::Graphics::MaterialNode* mTentacleMaterialNode;
	//WOWGE::Graphics::MeshNode* mPlaneMeshNode;
	WOWGE::Graphics::MeshNode* mSkySphereMeshNode;
	WOWGE::Graphics::ModelNode* mModelNode;
	//WOWGE::Graphics::MeshNode* mTentacleArmMeshNode;
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
	//WOWGE::Graphics::TextureNode* mPlaneTextureNode;
	WOWGE::Graphics::TextureNode* mSkySphereTextureNode;
	//WOWGE::Graphics::TextureNode* mTentacleArmTextureNode;
	//WOWGE::Graphics::TransformNode* mTentacleArmTransformNode;
	//WOWGE::Graphics::TransformNode* mPlaneTransformNode;
	WOWGE::Graphics::TransformNode* mSkySphereTransformNode;

	WOWGE::Graphics::MatrixStack mMatrixStack;

	std::vector<WOWGE::Graphics::Bone> mBones;
	std::vector<WOWGE::Maths::Matrix44> mBoneWorldTransform;

	WOWGE::Maths::Vector3 mTarget{ 0.0f, 50.0f, 0.0f };
};

#endif // !INCLUDED_GAMEAPP_H
