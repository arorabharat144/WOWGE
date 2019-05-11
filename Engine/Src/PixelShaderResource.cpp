#include "Precompiled.h"
#include "PixelShaderResource.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

PixelShaderResource::PixelShaderResource()
{

}

PixelShaderResource::~PixelShaderResource()
{

}

void PixelShaderResource::Initialize(std::string filePath)
{
	mPixelShader = std::make_unique<Graphics::PixelShader>();

	mPixelShader->Initialize(std::wstring{ filePath.begin(), filePath.end() }.c_str(), "PS", "ps_5_0");
}

void PixelShaderResource::Terminate()
{
	mPixelShader->Terminate();
}

void PixelShaderResource::Render()
{
	mPixelShader->Bind();
}