#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <Input/Inc/Input.h>

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

	WOWGE::Graphics::Mesh mSphereMesh;
	WOWGE::Graphics::Mesh mSkySphere;

	WOWGE::Graphics::MeshBuffer mSkySphereMeshBuffer;
	WOWGE::Graphics::MeshBuffer mSphereMeshBuffer;

	WOWGE::Graphics::VertexShader mVertexShader;
	WOWGE::Graphics::PixelShader mPixelShader;

	WOWGE::Graphics::Camera mCamera;

	WOWGE::Graphics::Texture mDiffuseTexture;
	WOWGE::Graphics::Texture mSpecularTexture;
	WOWGE::Graphics::Texture mDisplacementTexture;
	WOWGE::Graphics::Texture mNormalTexture;

	WOWGE::Maths::Vector3 planetTranslation;
	WOWGE::Maths::Vector3 planetScaling;

	//SceneStuff
	WOWGE::Graphics::CameraNode* mCameraNode;
	WOWGE::Graphics::LightNode* mLightNode;
	WOWGE::Graphics::MaterialNode* mMaterialNode;
	WOWGE::Graphics::MeshNode* mMeshNode;
	WOWGE::Graphics::SamplerNode* mSamplerNode;
	WOWGE::Graphics::SceneManager* mSceneManager;
	WOWGE::Graphics::ShaderNode* mShaderNode;
	WOWGE::Graphics::TextureNode* mDiffuseTextureNode;
	WOWGE::Graphics::TextureNode* mSpecularTextureNode;
	WOWGE::Graphics::TextureNode* mDisplacementTextureNode;
	WOWGE::Graphics::TextureNode* mNormalTextureNode;
	WOWGE::Graphics::TransformNode* mTransformNode;

	WOWGE::Graphics::MatrixStack mMatrixStack;

};

#endif // !INCLUDED_GAMEAPP_H
