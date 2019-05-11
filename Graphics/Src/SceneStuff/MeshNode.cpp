//====================================================================================================
// Filename:	MeshNode.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\MeshNode.h"

//#include "RenderMesh.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

//====================================================================================================
// Class Definitions
//====================================================================================================

MeshNode::MeshNode()
	: mpMeshBuffer(nullptr)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

MeshNode::~MeshNode()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

void MeshNode::OnRender()
{
	if (mpMeshBuffer != nullptr)
	{
		mpMeshBuffer->Render();
	}
}