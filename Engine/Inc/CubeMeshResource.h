#ifndef INCLUDED_ENGINE_CUBEMESHRESOURCE_H
#define INCLUDED_ENGINE_CUBEMESHRESOURCE_H

#include "PrimitiveMeshResource.h"

namespace WOWGE
{
namespace Engine
{

class CubeMeshResource : public PrimitiveMeshResource
{
public:
	CubeMeshResource();
	~CubeMeshResource() override;

	void CreateMesh() override;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_CUBEMESHRESOURCE_H
