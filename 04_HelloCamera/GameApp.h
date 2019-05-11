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

	// Homework: Graphics::MeshBuffer
	// - Add methods:
	// void Initialize(void* vertices, uint32_t vertexSize, uint32_t vertexCount,
	//				   uint32_t* indices, uint32_t indexCount);
	// void Initialize(void* vertices, uint32_t vertexSize, uint32_t vertexCount);
	// void Terminate();
	// void Render();

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
};

#endif // !INCLUDED_GAMEAPP_H
