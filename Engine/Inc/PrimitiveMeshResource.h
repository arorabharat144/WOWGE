#ifndef INCLUDED_ENGINE_PRIMITIVEMESHRESOURCE_H
#define INCLUDED_ENGINE_PRIMITIVEMESHRESOURCE_H

#include "Resource.h"

namespace WOWGE
{
namespace Engine
{

class PrimitiveMeshResource : public Resource
{
public:
	PrimitiveMeshResource();
	~PrimitiveMeshResource() override;

	void Initialize(std::string filePath = "") override;

	virtual void CreateMesh() {}

	void Terminate() override;

	Graphics::Mesh* GetMesh() const;

protected:
	std::unique_ptr<Graphics::Mesh> mMesh;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_PRIMITIVEMESHRESOURCE_H
