//====================================================================================================
// Filename:	TransformNode.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\TransformNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

//====================================================================================================
// Class Definitions
//====================================================================================================

TransformNode::TransformNode(MatrixStack& ms)
	: mMatrixStack(ms)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

TransformNode::~TransformNode()
{
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void TransformNode::Initialize(uint32_t bindIndex)
{
	mBindIndex = bindIndex;
	mConstantBuffer.Initialize();
}

//----------------------------------------------------------------------------------------------------

void TransformNode::Terminate()
{
	mConstantBuffer.Terminate();
}

//----------------------------------------------------------------------------------------------------

void TransformNode::OnUpdate(float deltaTime)
{
}

//----------------------------------------------------------------------------------------------------

void TransformNode::OnPreRender()
{
	mMatrixStack.PushMatrix(mTransform.GetTransformMatrix());
}

//----------------------------------------------------------------------------------------------------

void TransformNode::OnRender()
{
	ConstantBuffer constants;
	constants.matWorld = mMatrixStack.GetMatrixTranspose();
	mConstantBuffer.Set(constants);
	
	mConstantBuffer.BindVS(mBindIndex);
	mConstantBuffer.BindPS(mBindIndex);
}

//----------------------------------------------------------------------------------------------------

void TransformNode::OnPostRender()
{
	mMatrixStack.PopMatrix();
}