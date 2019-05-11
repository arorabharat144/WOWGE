#ifndef INCLUDED_GRAPHICS_MESHBUFFER_H
#define INCLUDED_GRAPHICS_MESHBUFFER_H

#include <Graphics/Inc/GraphicsSystem.h>

namespace WOWGE
{
namespace Graphics
{

class Mesh;
class SkinnedMesh;

class MeshBuffer
{
public:
	MeshBuffer();
	~MeshBuffer();

	void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);
	void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
	void Initialize(const Mesh& mesh);
	void Initialize(const SkinnedMesh& skinnedMesh);

	void Terminate();

	void Render() const; //Using index data

private:
	MeshBuffer(const MeshBuffer&) = delete;
	MeshBuffer& operator=(const MeshBuffer&) = delete;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

	uint32_t mVertexSize;
	int mIndexCount;
	int mVertexCount;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_MESHBUFFER_H
