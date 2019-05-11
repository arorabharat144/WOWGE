//====================================================================================================
// Filename:	TextureNode.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\TextureNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

//====================================================================================================
// Class Definitions
//====================================================================================================

TextureNode::TextureNode(ShaderStage shaderStage)
	: mShaderStage(shaderStage)
	, mpTexture(nullptr)
	, mIndex(0)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

TextureNode::~TextureNode()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

void TextureNode::OnRender()
{
	if (mpTexture != nullptr)
	{
		if (mShaderStage == ShaderStage::VertexShader)
		{
			mpTexture->BindVS(mIndex);
		}
		else if (mShaderStage == ShaderStage::PixelShader)
		{
			mpTexture->BindPS(mIndex);
		}
	}
}

void TextureNode::OnPostRender()
{
	//mIndex == 1 is the normal map
	if (mShaderStage == ShaderStage::PixelShader && mIndex != 1)
	{
		mpTexture->UnbindPS(mIndex);
	}

	if (mShaderStage == ShaderStage::VertexShader)
	{
		mpTexture->UnbindVS(mIndex);
	}
}