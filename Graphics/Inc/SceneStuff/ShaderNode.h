#ifndef INCLUDED_GRAPHICS_SHADERNODE_H
#define INCLUDED_GRAPHICS_SHADERNODE_H

//====================================================================================================
// Filename:	ShaderNode.h
// Description:	Class for a shader node.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "SceneNode.h"

#include "PixelShader.h"
#include "VertexShader.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

//class PixelShader;
//class VertexShader;

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{

class ShaderNode : public SceneNode
{
public:
	ShaderNode();
	~ShaderNode();

	void SetVertexShader(VertexShader* vertexShader) { mpVertexShader = vertexShader; }
	void SetPixelShader(PixelShader* pixelShader) { mpPixelShader = pixelShader; }

private:
	void OnRender() override;

private:
	VertexShader * mpVertexShader;
	PixelShader* mpPixelShader;
};

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_SHADERNODE_H