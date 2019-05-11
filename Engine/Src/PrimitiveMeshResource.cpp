#include "Precompiled.h"
#include "PrimitiveMeshResource.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

PrimitiveMeshResource::PrimitiveMeshResource()
{

}
PrimitiveMeshResource::~PrimitiveMeshResource()
{

}

void PrimitiveMeshResource::Initialize(std::string filePath)
{
	mMesh = std::make_unique<Graphics::Mesh>();

	CreateMesh();
}

void PrimitiveMeshResource::Terminate()
{
	mMesh->Destroy();
}

Graphics::Mesh* PrimitiveMeshResource::GetMesh() const
{
	return mMesh.get();
}