#ifndef INCLUDED_GRAPHICS_TERRAIN_HEADER
#define INCLUDED_GRAPHICS_TERRAIN_HEADER

#include "HeightMap.h"
#include "Mesh.h"
#include "MeshBuffer.h"

namespace WOWGE
{
namespace Graphics
{
class Terrain
{
public:
	Terrain();
	~Terrain();

	Terrain(const Terrain&) = delete;
	Terrain& operator=(const Terrain&) = delete;

	void Initialize(const char* fileName, uint32_t columns, uint32_t rows, float maxHeight);

	void Terminate();

	void Render() const;
private:
	HeightMap mHeightMap;
	Mesh mMesh;
	MeshBuffer mMeshBuffer;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_TERRAIN_HEADER
