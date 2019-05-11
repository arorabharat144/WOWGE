#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>

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

	// Homework: Graphics::MeshBuffer
	// - Add methods:
	// void Initialize(void* vertices, uint32_t vertexSize, uint32_t vertexCount,
	//				   uint32_t* indices, uint32_t indexCount);
	// void Initialize(void* vertices, uint32_t vertexSize, uint32_t vertexCount);
	// void Terminate();
	// void Render();

	WOWGE::Graphics::MeshBuffer mMeshBuffer;
	ID3D11Buffer* mConstantBuffer;

	WOWGE::Graphics::VertexShader mVertexShader;
	WOWGE::Graphics::PixelShader mPixelShader;
};

#endif // !INCLUDED_GAMEAPP_H
