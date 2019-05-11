#include "Precompiled.h"
#include "TextureResource.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

TextureResource::TextureResource()
{

}
TextureResource::~TextureResource()
{

}

void TextureResource::Initialize(std::string filePath)
{
	mTexture = std::make_unique<Graphics::Texture>();

	mTexture->Initialize(filePath.c_str());
}

void TextureResource::Terminate()
{
	mTexture->Terminate();
}

void TextureResource::Render()
{
	mTexture->BindVS(0);
	mTexture->BindPS(0);
}