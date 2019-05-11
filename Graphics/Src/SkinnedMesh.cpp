#include "Precompiled.h"
#include "SkinnedMesh.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

SkinnedMesh::SkinnedMesh() :
	mVertices(nullptr),
	mIndices(nullptr),
	mNumVertices(0),
	mNumIndices(0)
{

}

SkinnedMesh::~SkinnedMesh()
{
	ASSERT(mVertices == nullptr, "[SkinnedMesh] Vertices not freed!");
	ASSERT(mIndices == nullptr, "[SkinnedMesh] Indices not freed!");
}

void SkinnedMesh::Allocate(uint32_t numVertices, uint32_t numIndices)
{
	ASSERT(numVertices > 0, "[SkinnedMesh] Invalid count.");
	ASSERT(numIndices > 0, "[SkinnedMesh] Invalid count.");

	Destroy();

	mNumVertices = numVertices;
	mNumIndices = numIndices;

	mVertices = new BoneVertex[mNumVertices];
	mIndices = new uint32_t[mNumIndices];
}

void SkinnedMesh::Destroy()
{
	SafeDeleteArray(mVertices);
	SafeDeleteArray(mIndices);
	mVertices = 0;
	mIndices = 0;
}

BoneVertex& SkinnedMesh::GetVertex(uint32_t index)
{
	ASSERT(index < mNumVertices, "[SkinnedMesh] Invalid index.");
	return mVertices[index];
}

uint32_t& SkinnedMesh::GetIndex(uint32_t index)
{
	ASSERT(index < mNumIndices, "[SkinnedMesh] Invalid index.");
	return mIndices[index];
}