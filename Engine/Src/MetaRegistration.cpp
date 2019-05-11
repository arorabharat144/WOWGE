#include "Precompiled.h"
#include "MetaRegistration.h"

#include <Core/Inc/MetaRegistry.h>

#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"

void WOWGE::Engine::Meta::MetaRegister()
{
#define REGISTER(x) WOWGE::Core::Meta::MetaRegistry::Register(x::StaticGetMetaClass())

	REGISTER(CameraComponent);
	REGISTER(TransformComponent);
	REGISTER(ColliderComponent);
	REGISTER(MeshComponent);

#undef REGISTER
}