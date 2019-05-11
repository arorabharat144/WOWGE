#include "Precompiled.h"
#include "SphereMeshResource.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

SphereMeshResource::SphereMeshResource()
{

}
SphereMeshResource::~SphereMeshResource()
{

}

void SphereMeshResource::CreateMesh()
{
	Graphics::MeshBuilder::CreateSphere(*mMesh.get(), 200, 200);
}