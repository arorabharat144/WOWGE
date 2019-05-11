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

	ID3D11Buffer* mVertexBufferTriforce;
	ID3D11Buffer* mVertexBufferHeart;
	ID3D11Buffer* mVertexBufferStar;
	ID3D11Buffer* mVertexBufferTransformer;

	WOWGE::Graphics::VertexShader mVertexShader;
	WOWGE::Graphics::PixelShader mPixelShader;
};

#endif // !INCLUDED_GAMEAPP_H
