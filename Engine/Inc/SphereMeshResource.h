#ifndef INCLUDED_ENGINE_SPHEREMESHRESOURCE_H
#define INCLUDED_ENGINE_SPHEREMESHRESOURCE_H

#include "PrimitiveMeshResource.h"

namespace WOWGE
{
namespace Engine
{

class SphereMeshResource : public PrimitiveMeshResource
{
public:
	SphereMeshResource();
	~SphereMeshResource() override;

	void CreateMesh() override;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_SPHEREMESHRESOURCE_H
