//====================================================================================================
// Filename:	ShaderNode.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "SceneStuff\ShaderNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

//====================================================================================================
// Class Definitions
//====================================================================================================

ShaderNode::ShaderNode()
	: mpVertexShader(nullptr)
	, mpPixelShader(nullptr)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

ShaderNode::~ShaderNode()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

void ShaderNode::OnRender()
{
	if (mpVertexShader != nullptr)
	{
		//mpVertexShader->Bind(mGraphicsSystem);
		mpVertexShader->Bind();
	}
	if (mpPixelShader != nullptr)
	{
		//mpPixelShader->Bind(mGraphicsSystem);
		mpPixelShader->Bind();
	}
}