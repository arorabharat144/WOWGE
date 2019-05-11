#ifndef INCLUDED_GRAPHICS_LIGHTNODE_H
#define INCLUDED_GRAPHICS_LIGHTNODE_H

//====================================================================================================
// Filename:	LightNode.h
// Description:	Class for a light node.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Math\Inc\Vector4.h"
#include "ConstantBuffer.h"
#include "Light.h"
#include "SceneNode.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{

class LightNode : public SceneNode
{
public:
	struct ConstantBuffer
	{
		Maths::Vector4 direction;
		Maths::Vector4 ambient;
		Maths::Vector4 diffuse;
		Maths::Vector4 specular;
	};

	LightNode();
	~LightNode();

	void Initialize();
	void Terminate();

	Light& GetLight() { return mLight; }

private:
	void OnRender() override;

private:
	TypedConstantBuffer<ConstantBuffer> mConstantBuffer;

	Light mLight;
};

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_LIGHTNODE_H