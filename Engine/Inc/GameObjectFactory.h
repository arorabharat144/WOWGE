#ifndef INCLUDED_ENGINE_GAMEOBJECTFACTORY_H
#define INCLUDED_ENGINE_GAMEOBJECTFACTORY_H

#include "GameObject.h"

namespace WOWGE
{
namespace Engine
{
class GameObjectFactory
{
public:
	GameObjectFactory(GameOjectAllocator& allocator);

	GameObject* Create(const char* templateFileName);
	void Destroy(GameObject* gameObject);
private:
	GameOjectAllocator& mGameObjectAllocator;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_GAMEOBJECTFACTORY_H
