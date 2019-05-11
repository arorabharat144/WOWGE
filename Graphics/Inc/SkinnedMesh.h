#ifndef INCLUDED_GRAPHICS_SKINNEDMESH_HEADER
#define INCLUDED_GRAPHICS_SKINNEDMESH_HEADER

#include "VertexTypes.h"

namespace WOWGE
{
namespace Graphics
{
class SkinnedMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();

	void Allocate(uint32_t numVertices, uint32_t numIndices);
	void Destroy();

	BoneVertex& GetVertex(uint32_t index);
	uint32_t& GetIndex(uint32_t index);

	const BoneVertex* GetVertices() const { return mVertices; }
	const uint32_t* GetIndices() const { return mIndices; }

	uint32_t GetVertexCount() const { return mNumVertices; }
	uint32_t GetIndexCount() const { return mNumIndices; }
private:
	SkinnedMesh(const SkinnedMesh&) = delete;
	SkinnedMesh& operator=(const SkinnedMesh&) = delete;

	BoneVertex* mVertices;
	uint32_t* mIndices;

	uint32_t mNumVertices;
	uint32_t mNumIndices;
};


} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_MESH_HEADER
