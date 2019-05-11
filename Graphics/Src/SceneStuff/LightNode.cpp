//====================================================================================================
// Filename:	LightNode.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\LightNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

//====================================================================================================
// Class Definitions
//====================================================================================================

LightNode::LightNode()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

LightNode::~LightNode()
{
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void LightNode::Initialize()
{
	mConstantBuffer.Initialize();
}

//----------------------------------------------------------------------------------------------------

void LightNode::Terminate()
{
	mConstantBuffer.Terminate();
}

//----------------------------------------------------------------------------------------------------

void LightNode::OnRender()
{
	const Maths::Vector4& direction = mLight.direction;
	const Maths::Vector4& ambient = mLight.ambient;
	const Maths::Vector4& diffuse = mLight.diffuse;
	const Maths::Vector4& specular = mLight.specular;

	ConstantBuffer constants;
	constants.direction = Maths::Vector4(direction.x, direction.y, direction.z, direction.w);
	constants.ambient = Maths::Vector4(ambient.x, ambient.y, ambient.z, ambient.w);
	constants.diffuse = Maths::Vector4(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
	constants.specular = Maths::Vector4(specular.x, specular.y, specular.z, specular.w);
	mConstantBuffer.Set(constants);
	
	mConstantBuffer.BindVS(1);
	mConstantBuffer.BindPS(1);
}