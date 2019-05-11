#ifndef INCLUDED_GRAPHICS_TEXTURENODE_H
#define INCLUDED_GRAPHICS_TEXTURENODE_H

//====================================================================================================
// Filename:	TextureNode.h
// Description:	Class for a texture node.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Core\Inc\Common.h"
#include "SceneNode.h"
#include "ShaderStage.h"

#include "Texture.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

//class Texture;

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{
class TextureNode : public SceneNode
{
public:
	TextureNode(ShaderStage shaderStage);
	~TextureNode();

	void SetTexture(Texture* texture) { mpTexture = texture; }
	Texture* GetTexture() const { return mpTexture; }

	void SetIndex(uint32_t index) { mIndex = index; }
	uint32_t GetIndex() const { return mIndex; }

private:
	void OnRender() override;
	void OnPostRender() override;

private:
	Texture * mpTexture;
	uint32_t mIndex;

	ShaderStage mShaderStage;
};

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_TEXTURENODE_H