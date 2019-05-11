//====================================================================================================
// Filename:	MaterialNode.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\MaterialNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

//====================================================================================================
// Class Definitions
//====================================================================================================

MaterialNode::MaterialNode()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

MaterialNode::~MaterialNode()
{
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void MaterialNode::Initialize()
{
	//mConstantBuffer.Initialize(mGraphicsSystem);
	mConstantBuffer.Initialize();
}

//----------------------------------------------------------------------------------------------------

void MaterialNode::Terminate()
{
	mConstantBuffer.Terminate();
}

//----------------------------------------------------------------------------------------------------

void MaterialNode::OnRender()
{
	const Maths::Vector4& ambient = mMaterial.ambient;
	const Maths::Vector4& diffuse = mMaterial.diffuse;
	const Maths::Vector4& specular = mMaterial.specular;
	const Maths::Vector4& emissive = mMaterial.emissive;

	ConstantBuffer constants;
	constants.ambient = Maths::Vector4(ambient.x, ambient.y, ambient.z, ambient.w);
	constants.diffuse = Maths::Vector4(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
	constants.specular = Maths::Vector4(specular.x, specular.y, specular.z, specular.w);
	constants.emissive = Maths::Vector4(emissive.x, emissive.y, emissive.z, emissive.w);
	constants.power = mMaterial.power;
	mConstantBuffer.Set(constants);
	
	mConstantBuffer.BindVS(2);
	mConstantBuffer.BindPS(2);
}