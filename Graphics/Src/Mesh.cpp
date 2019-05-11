#include "Precompiled.h"
#include "Mesh.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

Mesh::Mesh():
	mVertices(nullptr),
	mIndices(nullptr),
	mNumVertices(0),
	mNumIndices(0)
{

}

Mesh::~Mesh()
{
	ASSERT(mVertices == nullptr, "[Mesh] Vertices not freed!");
	ASSERT(mIndices == nullptr, "[Mesh] Indices not freed!");
}

void Mesh::Allocate(uint32_t numVertices, uint32_t numIndices)
{
	ASSERT(numVertices > 0, "[Mesh] Invalid count.");
	ASSERT(numIndices > 0, "[Mesh] Invalid count.");

	Destroy();

	mNumVertices = numVertices;
	mNumIndices = numIndices;

	mVertices = new Vertex[mNumVertices];
	mIndices = new uint32_t[mNumIndices];
}

void Mesh::Destroy()
{
	SafeDeleteArray(mVertices);
	SafeDeleteArray(mIndices);
	mVertices = 0;
	mIndices = 0;
}

Vertex& Mesh::GetVertex(uint32_t index)
{
	ASSERT(index < mNumVertices, "[Mesh] Invalid index.");
	return mVertices[index];
}

uint32_t& Mesh::GetIndex(uint32_t index)
{
	ASSERT(index < mNumIndices, "[Mesh] Invalid index.");
	return mIndices[index];
}