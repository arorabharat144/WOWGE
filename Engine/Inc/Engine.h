#ifndef INCLUDED_ENGINE_H
#define INCLUDED_ENGINE_H

#include "Common.h"

#include "Component.h"
#include "GameApp.h"
#include "GameObject.h"
#include "GameObjectFactory.h"

#include "MetaRegistration.h"

#include "State.h"
#include "StateMachine.h"

//Components
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"

//Resource Managers
#include "Resource.h"
#include "ResourceManager.h"
#include "CubeMeshResource.h"
#include "PixelShaderResource.h"
#include "PrimitiveMeshResource.h"
#include "SphereMeshResource.h"
#include "TextureResource.h"
#include "VertexShaderResource.h"


#include "World.h"

#include "XMLReader.h"

#endif // !INCLUDED_ENGINE_H
