#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core/Inc/Core.h>
#include <Audio/Inc/Audio.h>
#include <Graphics/Inc/Graphics.h>
#include <Input/Inc/Input.h>

#include <External/ImGui/Inc/imgui.h>

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

	WOWGE::Graphics::Mesh mPlane;
	WOWGE::Graphics::Texture mPlaneTexture;
	WOWGE::Graphics::MeshBuffer mPlaneMeshBuffer;

	WOWGE::Graphics::MeshBuffer mDebugQuadMeshBuffer;

	WOWGE::Graphics::VertexShader mDepthVertexShader;
	WOWGE::Graphics::PixelShader mDepthPixelShader;

	WOWGE::Graphics::VertexShader mShadowMappingVertexShader;
	WOWGE::Graphics::PixelShader mShadowMappingPixelShader;

	WOWGE::Graphics::VertexShader mUIVertexShader;
	WOWGE::Graphics::PixelShader mUIPixelShader;

	WOWGE::Graphics::VertexShader mGroundVertexShader;
	WOWGE::Graphics::PixelShader mGroundPixelShader;

	WOWGE::Graphics::Mesh mSkySphere;
	WOWGE::Graphics::MeshBuffer mSkySphereMeshBuffer;
	WOWGE::Graphics::Texture mSkyTexture;

	WOWGE::Graphics::VertexShader mSkySphereVertexShader;
	WOWGE::Graphics::PixelShader mSkySpherePixelShader;

	WOWGE::Graphics::Camera mCamera;
	WOWGE::Graphics::Camera mLightCamera;

	WOWGE::Graphics::DepthMap mDepthMap;

	WOWGE::Graphics::RasterizerState mRasterizerState;

	WOWGE::Graphics::RenderTarget* mRenderTarget;

	WOWGE::Graphics::AnimationController mSharkManAnimationController;
	WOWGE::Graphics::AnimationController mPeasantGirlAnimationController;
	WOWGE::Graphics::AnimationController mPearlAnimationController;
	WOWGE::Graphics::AnimationController mAndromedaAnimationController;
	WOWGE::Graphics::AnimationController mEveAnimationController;
	WOWGE::Graphics::AnimationController mShaeAnimationController;
	WOWGE::Graphics::AnimationController mStefaniAnimationController;

	WOWGE::Audio::SoundEffectController mSoundEffectController;

	//SceneStuff
	WOWGE::Graphics::CameraNode* mCameraNode;
	WOWGE::Graphics::LightNode* mLightNode;
	WOWGE::Graphics::MaterialNode* mPlaneMaterialNode;
	WOWGE::Graphics::MeshNode* mPlaneMeshNode;
	WOWGE::Graphics::MeshNode* mDebugQuadMeshNode;
	WOWGE::Graphics::MeshNode* mSkySphereMeshNode{ nullptr };
	WOWGE::Graphics::ModelNode* mSharkManNode{ nullptr };
	WOWGE::Graphics::ModelNode* mPeasantGirlNode{ nullptr };
	WOWGE::Graphics::ModelNode* mPearlNode{ nullptr };
	WOWGE::Graphics::ModelNode* mAndromedaNode{ nullptr };
	WOWGE::Graphics::ModelNode* mEveNode{ nullptr };
	WOWGE::Graphics::ModelNode* mShaeNode{ nullptr };
	WOWGE::Graphics::ModelNode* mStefaniNode{ nullptr };
	WOWGE::Graphics::RasterizerStateNode* mRasterizerStateNode;
	WOWGE::Graphics::DepthMapNode* mDepthMapNode;
	WOWGE::Graphics::DepthMapNode* mDepthMapNode2ndPass;
	WOWGE::Graphics::DepthMapNode* mDebugDepthMapNode;
	WOWGE::Graphics::SamplerNode* mSamplerNode;
	WOWGE::Graphics::SamplerNode* mGroundSamplerNode;
	WOWGE::Graphics::SamplerNode* mDepthSamplerNode;
	WOWGE::Graphics::SamplerNode* mSkySphereSamplerNode{ nullptr };
	WOWGE::Graphics::SamplerNode* mUISamplerNode;
	WOWGE::Graphics::SceneManager* mSceneManager;
	WOWGE::Graphics::ShaderNode* mDepthMapShaderNode;
	WOWGE::Graphics::ShaderNode* mShadowMapShaderNode;
	WOWGE::Graphics::ShaderNode* mGroundShaderNode;
	WOWGE::Graphics::ShaderNode* mShaderNodeSkySphere{ nullptr };
	WOWGE::Graphics::ShaderNode* mUIShaderNode;
	WOWGE::Graphics::TextureNode* mPlaneTextureNode;
	WOWGE::Graphics::TextureNode* mSkySphereTextureNode{ nullptr };
	WOWGE::Graphics::TransformNode* mPlaneTransformNode;
	WOWGE::Graphics::TransformNode* mSkySphereTransformNode{ nullptr };

	WOWGE::Graphics::MatrixStack mMatrixStack;
};
// float depth = DepthMap.Sample(SamplerLinear, input.texcoord).r;
// float near = 1.0f;
// float far = 500.0f
// float linearDepth = (2.0f * near) / (far + near - depth * (far - near));
// return linearDepth

#endif // !INCLUDED_GAMEAPP_H
