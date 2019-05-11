#ifndef INCLUDED_GRAPHICS_MATERIALNODE_H
#define INCLUDED_GRAPHICS_MATERIALNODE_H

//====================================================================================================
// Filename:	MaterialNode.h
// Description:	Class for a material node.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Math\Inc\Vector4.h"
#include "ConstantBuffer.h"
#include "Material.h"
#include "SceneNode.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{

class MaterialNode : public SceneNode
{
public:
	struct ConstantBuffer
	{
		Maths::Vector4 ambient;
		Maths::Vector4 diffuse;
		Maths::Vector4 specular;
		Maths::Vector4 emissive;
		float power;
	};

	MaterialNode();
	~MaterialNode();

	void Initialize();
	void Terminate();

	Material& GetMaterial() 
	{ 
		return mMaterial; 
	}

private:
	void OnRender() override;

private:
	TypedConstantBuffer<ConstantBuffer> mConstantBuffer;

	Material mMaterial;
};

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_MATERIALNODE_H