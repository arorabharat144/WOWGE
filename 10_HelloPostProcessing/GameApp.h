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
	WOWGE::Graphics::Texture mSkyTexture;

	WOWGE::Graphics::Mesh mPlane;

	WOWGE::Graphics::MeshBuffer mSkySphereMeshBuffer;

	WOWGE::Graphics::MeshBuffer mPlaneMeshBuffer;

	WOWGE::Graphics::VertexShader mSkySphereVertexShader;
	WOWGE::Graphics::PixelShader mSkySpherePixelShader;

	WOWGE::Graphics::VertexShader mVertexShader;
	WOWGE::Graphics::PixelShader mPixelShader;

	WOWGE::Graphics::VertexShader mVertexShaderPostProcessing;
	WOWGE::Graphics::PixelShader mPixelShaderPostProcessing;

	WOWGE::Graphics::Camera mCamera;

	WOWGE::Graphics::RasterizerState mRasterizerState;

	WOWGE::Maths::Vector3 planetTranslation;
	WOWGE::Maths::Vector3 planetScaling;

	WOWGE::Graphics::ModelNode* mModelNode;
	WOWGE::Graphics::AnimationController mLiamAnimationController;

	WOWGE::Graphics::RenderTarget* mRenderTarget;

	//SceneStuff
	WOWGE::Graphics::CameraNode* mCameraNode;
	WOWGE::Graphics::LightNode* mLightNode;
	WOWGE::Graphics::MeshNode* mPlaneMeshNode;
	WOWGE::Graphics::MeshNode* mSkySphereMeshNode;
	WOWGE::Graphics::RasterizerStateNode* mRasterizerStateNode;
	WOWGE::Graphics::RenderTargetNode* mRenderTargetNode;
	WOWGE::Graphics::RenderTargetNode* mRenderTargetNode2ndPass;
	WOWGE::Graphics::SamplerNode* mSkySphereSamplerNode;
	WOWGE::Graphics::SamplerNode* mSamplerNode;
	WOWGE::Graphics::SceneManager* mSceneManager;
	WOWGE::Graphics::ShaderNode* mShaderNodeSkySphere;
	WOWGE::Graphics::ShaderNode* mShaderNode;
	WOWGE::Graphics::ShaderNode* mShaderNodePostProcessing;
	WOWGE::Graphics::TextureNode* mTextureNode;

	WOWGE::Graphics::TransformNode* mSkySphereTransformNode;

	WOWGE::Graphics::MatrixStack mMatrixStack;

};

#endif // !INCLUDED_GAMEAPP_H
