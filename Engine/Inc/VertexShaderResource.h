#ifndef INCLUDED_ENGINE_VERTEXSHADERRESOURCE_H
#define INCLUDED_ENGINE_VERTEXSHADERRESOURCE_H

#include "Resource.h"

namespace WOWGE
{
namespace Engine
{

class VertexShaderResource : public Resource
{
public:
	VertexShaderResource();
	~VertexShaderResource() override;

	void Initialize(std::string filePath = "") override;

	void Terminate() override;

	void Render() override;

private:
	std::unique_ptr<Graphics::VertexShader> mVertexShader;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_VERTEXSHADERRESOURCE_H
