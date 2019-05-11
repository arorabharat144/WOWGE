#include "Precompiled.h"
#include "VertexShaderResource.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

VertexShaderResource::VertexShaderResource()
{

}

VertexShaderResource::~VertexShaderResource()
{

}

void VertexShaderResource::Initialize(std::string filePath)
{
	mVertexShader = std::make_unique<Graphics::VertexShader>();

	mVertexShader->Initialize(std::wstring{ filePath.begin(), filePath.end() }.c_str(), "VS", "vs_5_0", Graphics::Vertex::Format);
}

void VertexShaderResource::Terminate()
{
	mVertexShader->Terminate();
}

void VertexShaderResource::Render()
{
	mVertexShader->Bind();
}