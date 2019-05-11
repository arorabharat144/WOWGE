#ifndef INCLUDED_GRAPHICS_GRAPHICSUTIL_H
#define INCLUDED_GRAPHICS_GRAPHICSUTIL_H

#include "Mesh.h"

namespace WOWGE
{
namespace Graphics
{

class GraphicsUtil
{
public:
	static void ComputeNormals(Mesh& mesh);

	static void ComputeNormals(Vertex* vertices, uint32_t numVertices, uint32_t* indices, uint32_t numIndices);
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_GRAPHICSUTIL_H
