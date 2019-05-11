#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

#include <Core/Inc/MetaClass.h>

using namespace WOWGE;
using namespace WOWGE::Engine;

GameObject::GameObject() :
	mName{ "GameObject" }
{

}

GameObject::~GameObject()
{

}

void GameObject::Initialize()
{
	for (auto& component : mComponents)
	{
		component->Initialize();
	}
}
void GameObject::Terminate()
{
	for (auto& component : mComponents)
	{
		component->Terminate();
	}
}

void GameObject::Update(float deltaTime)
{
	for (auto& component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void GameObject::Render()
{
	for (auto& component : mComponents)
	{
		component->Render();
	}
}

Component* GameObject::AddComponent(const Core::Meta::MetaClass* metaClass)
{
	mComponents.emplace_back(static_cast<Component*>(metaClass->Create()));
	auto& newComponent{ mComponents.back() };
	newComponent->mGameObject = this;
	return newComponent.get();
}

Component* GameObject::GetComponent(const Core::Meta::MetaClass* metaClass)
{
	Component* result{ nullptr };
	for (auto& component : mComponents)
	{
		if (component->GetMetaClass() == metaClass)
		{
			result = component.get();
			break;
		}
	}
	return result;
}

Component* GameObject::GetComponentAt(uint32_t index)
{
	return mComponents[index].get();
}