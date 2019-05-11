#ifndef INCLUDED_GRAPHICS_MESHNODE_H
#define INCLUDED_GRAPHICS_MESHNODE_H

//====================================================================================================
// Filename:	MeshNode.h
// Description:	Class for a mesh node.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MeshBuffer.h"
#include "SceneNode.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

//class RenderMesh;
//class MeshBuffer;

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{

class MeshNode : public SceneNode
{
public:
	MeshNode();
	~MeshNode();

	//void SetRenderMesh(RenderMesh* mesh) { mpMesh = mesh; }
	//RenderMesh* GetRenderMesh() const { return mpMesh; }

	void SetMesh(MeshBuffer* mesh) { mpMeshBuffer = mesh; }
	MeshBuffer* GetMesh() const { return mpMeshBuffer; }

private:
	void OnRender() override;

private:
	//RenderMesh * mpMesh;
	MeshBuffer * mpMeshBuffer;
};

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_MESHNODE_H