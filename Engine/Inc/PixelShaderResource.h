#ifndef INCLUDED_ENGINE_PIXELSHADERRESOURCE
#define INCLUDED_ENGINE_PIXELSHADERRESOURCE

#include "Resource.h"

namespace WOWGE
{
namespace Engine
{

class PixelShaderResource : public Resource
{
public:
	PixelShaderResource();
	~PixelShaderResource() override;

	void Initialize(std::string filePath = "") override;

	void Terminate() override;

	void Render() override;

private:
	std::unique_ptr<Graphics::PixelShader> mPixelShader;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_PIXELSHADERRESOURCE
