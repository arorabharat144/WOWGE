#include "Precompiled.h"
#include "CubeMeshResource.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

CubeMeshResource::CubeMeshResource()
{

}
CubeMeshResource::~CubeMeshResource()
{

}
void CubeMeshResource::CreateMesh()
{
	Graphics::MeshBuilder::CreateCube(*mMesh.get());
}