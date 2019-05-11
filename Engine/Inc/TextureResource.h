#ifndef INCLUDED_ENGINE_TEXTURERESOURCE_H
#define INCLUDED_ENGINE_TEXTURERESOURCE_H

#include "Resource.h"

namespace WOWGE
{
namespace Engine
{

class TextureResource : public Resource
{
public:
	TextureResource();
	~TextureResource() override;

	void Initialize(std::string filePath = "") override;

	void Terminate() override;

	void Render() override;
private:
	std::unique_ptr<Graphics::Texture> mTexture;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_TEXTURERESOURCE_H
