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
		WOWGE::Maths::Matrix44 wvp;
		WOWGE::Maths::Vector4 cameraPosition;
		WOWGE::Maths::Vector4 lightDirection;
		WOWGE::Maths::Vector4 lightAmbient;
		WOWGE::Maths::Vector4 lightDiffuse;
		WOWGE::Maths::Vector4 lightSpecular;
		WOWGE::Maths::Vector4 materialAmbient;
		WOWGE::Maths::Vector4 materialDiffuse;
		WOWGE::Maths::Vector4 materialSpecular;
		WOWGE::Maths::Vector4 materialEmissive;
		float materialPower;
	};
	typedef WOWGE::Graphics::TypedConstantBuffer<TransformData> TransformCB;

	WOWGE::Graphics::Mesh mSphereMesh;
	WOWGE::Graphics::Mesh mSkySphere;

	WOWGE::Graphics::MeshBuffer mSkySphereMeshBuffer;
	WOWGE::Graphics::MeshBuffer mSphereMeshBuffer;

	TransformCB mConstantBuffer;
	WOWGE::Graphics::VertexShader mVertexShader;
	WOWGE::Graphics::PixelShader mPixelShader;

	WOWGE::Graphics::Camera mCamera;

	WOWGE::Graphics::Light mLight;

	WOWGE::Graphics::Material mMaterial;
	WOWGE::Graphics::Material mSunMaterial;

	WOWGE::Graphics::Texture mTexture[11];
	WOWGE::Graphics::Sampler mAnisotropicSamplerUsingWrap;

	WOWGE::Maths::Vector3 planetTranslation;
	WOWGE::Maths::Vector3 planetScaling;

};

#endif // !INCLUDED_GAMEAPP_H
