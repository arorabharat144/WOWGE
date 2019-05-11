#ifndef INCLUDED_GRAPHICS_MESHBUILDER_HEADER
#define INCLUDED_GRAPHICS_MESHBUILDER_HEADER

#include "Mesh.h"

namespace WOWGE
{
namespace Graphics
{

class MeshBuilder
{
public:
	static void CreateCube(Mesh& mesh);
	static void CreateSphere(Mesh& mesh, uint32_t stacks, uint32_t slices);
	static void CreateCylinder(Mesh& mesh, uint32_t stacks, uint32_t slices);
	static void CreateSkySphere(Mesh& mesh, uint32_t stacks, uint32_t slices);
	static void CreatePostProcessingPlane(Mesh& mesh);
	static void CreatePlane(Mesh& mesh);
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_MESHBUILDER_HEADER

