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

	WOWGE::Graphics::VertexShader mVertexShader;
	WOWGE::Graphics::PixelShader mPixelShader;

	WOWGE::Graphics::Camera mCamera;

	WOWGE::Graphics::RasterizerState mRasterizerState;

	WOWGE::Maths::Vector3 planetTranslation;
	WOWGE::Maths::Vector3 planetScaling;

	//SceneStuff
	WOWGE::Graphics::CameraNode* mCameraNode;
	WOWGE::Graphics::LightNode* mLightNode;
	WOWGE::Graphics::RasterizerStateNode* mRasterizerStateNode;
	WOWGE::Graphics::SamplerNode* mSamplerNode;
	WOWGE::Graphics::SceneManager* mSceneManager;
	WOWGE::Graphics::ShaderNode* mShaderNode;

	WOWGE::Graphics::ModelNode* mModelNode;
	WOWGE::Graphics::ModelNode* mModelNode2;
	WOWGE::Graphics::ModelNode* mModelNode3;
	WOWGE::Graphics::ModelNode* mModelNode4;

	WOWGE::Graphics::MatrixStack mMatrixStack;

};

#endif // !INCLUDED_GAMEAPP_H
