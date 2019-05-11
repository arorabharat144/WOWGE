#ifndef INCLUDED_GRAPHICS_SKINNEDMESHBUILDER_HEADER
#define INCLUDED_GRAPHICS_SKINNEDMESHBUILDER_HEADER

#include "SkinnedMesh.h"

namespace WOWGE
{
namespace Graphics
{

class SkinnedMeshBuilder
{
public:
	static void CreateCylinder(SkinnedMesh& mesh, uint32_t stacks, uint32_t slices, float height, float radius);
	static void CreatePlane(SkinnedMesh& mesh);
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_SKINNEDMESHBUILDER_HEADER

