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

	struct TransformData
	{
		WOWGE::Maths::Matrix44 world;
		WOWGE::Maths::Matrix44 view;
		WOWGE::Maths::Matrix44 projection;
	};
	typedef WOWGE::Graphics::TypedConstantBuffer<TransformData> TransformCB;

	WOWGE::Graphics::MeshBuffer mMeshBuffer;

	TransformCB mConstantBuffer;
	WOWGE::Graphics::VertexShader mVertexShader;
	WOWGE::Graphics::PixelShader mPixelShader;

	WOWGE::Graphics::Camera mCamera;

	WOWGE::Graphics::Texture mCubeTexture;
	WOWGE::Graphics::Texture mWallTexture;

	WOWGE::Graphics::Sampler mPointSamplerUsingBorder;
	WOWGE::Graphics::Sampler mLinearSamplerUsingClamp;
	WOWGE::Graphics::Sampler mAnisotropicSamplerUsingMirror;
	WOWGE::Graphics::Sampler mAnisotropicSamplerUsingWrap;
};

#endif // !INCLUDED_GAMEAPP_H
