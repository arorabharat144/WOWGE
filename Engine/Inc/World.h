#ifndef INCLUDED_ENGINE_WORLD_H
#define INCLUDED_ENGINE_WORLD_H

#include "GameObjectFactory.h"

namespace WOWGE
{
namespace Engine
{

class ResourceManager;

class World
{
public:
	void Initialize(uint32_t capacity);
	void Terminate();

	void LoadLevel(const char* levelFileName);

	GameObjectHandle Create(const char* templateFileName, const char* name);
	GameObjectHandle Find(const char* name);
	void Destroy(GameObjectHandle handle);

	void Update(float deltaTime);

	void Render();

	ResourceManager* GetResourceManager() const;

private:
	void DestroyInternal(GameObject* gameObject);
	void ProcessDestroyList();

private:
	using GameObjectList = std::vector<GameObject*>;

	std::unique_ptr<GameOjectAllocator> mGameObjectAllocator;
	std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;
	std::unique_ptr<GameObjectFactory> mGameObjectFactory;

	std::unique_ptr<ResourceManager> mResourceManager;

	GameObjectList mUpdateList;
	GameObjectList mDestroyList;
	bool mInitialized{ false };
	bool mUpdating{ false };
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_WORLD_H
