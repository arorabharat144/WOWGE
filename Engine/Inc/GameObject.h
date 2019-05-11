#ifndef INCLUDED_ENGINE_GAMEOBJECT_H
#define INCLUDED_ENGINE_GAMEOBJECT_H

#include <Core/Inc/HandlePool.h>
#include <Core/Inc/TypedAllocator.h>

namespace WOWGE
{
namespace Engine
{

class GameObject;
class Component;
class World;

namespace Meta
{
	class MetaClass;
}

using GameObjectHandle = Core::Handle<GameObject>;
using GameObjectHandlePool = Core::HandlePool<GameObject>;
using GameOjectAllocator = Core::TypedAllocator<GameObject>;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Initialize();
	void Terminate();

	void Update(float deltaTime);
	void Render();

	template<class T>
	typename std::enable_if_t<std::is_base_of_v<Component, T>, T*>
		AddComponent();

	Component* AddComponent(const Core::Meta::MetaClass* metaClass);
	Component* GetComponent(const Core::Meta::MetaClass* metaClass);

	template<class T>
	T* GetComponent();

	Component* GetComponentAt(uint32_t index);
	size_t GetComponentCount() const { return mComponents.size(); }

	World& GetWorld() const { return *mWorld; }
	GameObjectHandle GetHandle() const { return mHandle; }

	const std::string& GetName() const { return mName; }
private:
	friend class World;
	using Components = std::vector<std::unique_ptr<Component>>;

	World* mWorld{ nullptr };
	std::string mName;
	GameObjectHandle mHandle;
	Components mComponents;
};

template<class T>
typename std::enable_if_t<std::is_base_of_v<Component, T>, T*>
GameObject::AddComponent()
{
	mComponents.emplace_back(std::make_unique<T>());

	auto& newComponent{ mComponents.back() };
	newComponent->mGameObject = this;
	return static_cast<T*>(newComponent.get());
}


template<class T>
T* GameObject::GetComponent()
{
	T* result{ nullptr };
	for (auto& component : mComponents)
	{
		result = Core::Meta::DynamicCast<Component, T>(component.get());
		if (result != nullptr)
		{
			break;
		}
	}
	return result;
}

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_GAMEOBJECT_H
