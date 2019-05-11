#include "Precompiled.h"
#include "World.h"

#include "ResourceManager.h"

#include "PixelShaderResource.h"
#include "TextureResource.h"
#include "VertexShaderResource.h"

#include "CubeMeshResource.h"
#include "SphereMeshResource.h"

#include "XMLReader.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

void World::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "[World] World already initialized");

	mGameObjectAllocator = std::make_unique<GameOjectAllocator>(capacity);
	mGameObjectHandlePool = std::make_unique<GameObjectHandlePool>(capacity);
	mGameObjectFactory = std::make_unique<GameObjectFactory>(*mGameObjectAllocator);

	mResourceManager = std::make_unique<ResourceManager>();

	mResourceManager->AddResource<CubeMeshResource>("Cube");
	mResourceManager->GetResource("Cube")->Initialize();
	mResourceManager->AddResource<SphereMeshResource>("Sphere");
	mResourceManager->GetResource("Sphere")->Initialize();

	mInitialized = true;
}

void World::Terminate()
{
	if (!mInitialized)
	{
		return;
	}

	ASSERT(!mUpdating, "[World] Cannot terminate world during update");

	// Destroy all active objects
	mUpdating = true;
	std::for_each(mUpdateList.begin(), mUpdateList.end(), [this](GameObject* gameObject)
	{
		Destroy(gameObject->GetHandle());
	});
	mUpdating = false;
	mUpdateList.clear();

	//Now destroy everything
	ProcessDestroyList();

	mGameObjectFactory.reset();
	mGameObjectHandlePool.reset();
	mGameObjectAllocator.reset();

	mResourceManager->Terminate();

	mInitialized = false;
}

void World::LoadLevel(const char* levelFileName)
{
	rapidxml::file<> levelXMLFile{ levelFileName };
	rapidxml::xml_document<> levelXMLDocument;
	levelXMLDocument.parse<0>(levelXMLFile.data());

	rapidxml::xml_node<>* levelXMLNode{ levelXMLDocument.first_node() };
	if (strcmp(levelXMLNode->name(), "Level") == 0)
	{
		rapidxml::xml_node<>* resourcesNode{ levelXMLNode->first_node() };

		if (strcmp(resourcesNode->name(), "Resources") == 0)
		{
			rapidxml::xml_node<>* resourceNode{ resourcesNode->first_node() };
			while (resourceNode != nullptr)
			{
				std::string resourceName;
				if (strcmp(resourceNode->name(), "VertexShader") == 0)
				{
					std::string vertexShaderName{ resourceNode->value() };
					vertexShaderName += "_V";
					resourceName = mResourceManager->AddResource<VertexShaderResource>(vertexShaderName);
					mResourceManager->GetResource(resourceName)->Initialize(resourceNode->value());
				}
				else if (strcmp(resourceNode->name(), "PixelShader") == 0)
				{
					std::string pixelShaderName{ resourceNode->value() };
					pixelShaderName += "_P";
					resourceName = mResourceManager->AddResource<PixelShaderResource>(pixelShaderName);
					mResourceManager->GetResource(resourceName)->Initialize(resourceNode->value());
				}
				else if (strcmp(resourceNode->name(), "Texture") == 0)
				{
					resourceName = mResourceManager->AddResource<TextureResource>(resourceNode->value());
				}

				mResourceManager->GetResource(resourceName)->Initialize(resourceNode->value());

				resourceNode = resourceNode->next_sibling();
			}
		}

		rapidxml::xml_node<>* gameObjectsNode{ resourcesNode->next_sibling() };
		if (strcmp(gameObjectsNode->name(), "GameObjects") == 0)
		{
			rapidxml::xml_node<>* gameObjectNode{ gameObjectsNode->first_node() };
			while (gameObjectNode != nullptr)
			{
				GameObjectHandle handle{ Create(gameObjectNode->first_node()->value(), gameObjectNode->first_attribute()->value()) };

				rapidxml::xml_node<>* overridenComponent{ gameObjectNode->first_node()->next_sibling() };
				while (overridenComponent != nullptr)
				{
					std::string componentName{ overridenComponent->name() };
					const Core::Meta::MetaClass* metaClass{ Core::Meta::MetaRegistry::FindMetaClass(componentName.c_str()) };
					if (metaClass != nullptr)
					{
						Component* component{ handle.Get()->GetComponent(metaClass) };
						const rapidxml::xml_node<>* data{ overridenComponent->first_node() };
						VERIFY(XMLReader::Read(component, metaClass, data), "Failed to write to component %s", metaClass->GetName());
					}

					overridenComponent = overridenComponent->next_sibling();
				}

				gameObjectNode = gameObjectNode->next_sibling();
			}
		}
	}
}

GameObjectHandle World::Create(const char* templateFileName, const char* name)
{
	auto gameObject{ mGameObjectFactory->Create(templateFileName) };
	ASSERT(gameObject != nullptr, "[World] Failed to create game object from template %s", templateFileName);

	// Register with the handle pool
	auto handle{ mGameObjectHandlePool->Register(gameObject) };

	// Initialize the game object
	gameObject->mWorld = this;
	gameObject->mHandle = handle;
	gameObject->mName = name;
	gameObject->Initialize();

	// Add game object to the update list
	mUpdateList.emplace_back(gameObject);

	return handle;
}

GameObjectHandle World::Find(const char* name)
{
	for (uint32_t i{ 0 }; i < mUpdateList.size(); ++i)
	{
		if (mUpdateList[i]->GetName().compare(name) == 0)
		{
			return mUpdateList[i]->GetHandle();
		}
	}

	return GameObjectHandle{};
}

void World::Destroy(GameObjectHandle handle)
{
	// If handle is invalid, nothing to do so just exit
	if (!handle.IsValid())
	{
		return;
	}

	//Cache the pointer and unregister with the handle pool
	GameObject* gameObject{ handle.Get() };
	mGameObjectHandlePool->Unregister(handle);

	// Check if we can destroy it now
	if (!mUpdating)
	{
		DestroyInternal(gameObject);
	}
	else
	{
		mDestroyList.emplace_back(gameObject);
	}
}

void World::Update(float deltaTime)
{
	ASSERT(!mUpdating, "[World] Already updating the world");

	// Lock the update list
	mUpdating = true;

	// Re-compute size in case new objects are added to the update list during iteration
	for (uint32_t i{ 0 }; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject{ mUpdateList[i] };
		if (gameObject->GetHandle().IsValid())
		{
			gameObject->Update(deltaTime);
		}
	}

	// Unlock the update list
	mUpdating = false;

	// Now we can safely destroy objects
	ProcessDestroyList();
}

void World::Render()
{
	for (auto gameObject : mUpdateList)
	{
		gameObject->Render();
	}
}

void World::DestroyInternal(GameObject* gameObject)
{
	ASSERT(!mUpdating, "[World] Cannot destroy game objects during update");

	// If pointer is invalid, nothing to do so just exit
	if (gameObject == nullptr)
	{
		return;
	}

	// First remove it from the update list
	auto iter{ std::find(mUpdateList.begin(), mUpdateList.end(), gameObject) };
	if (iter != mUpdateList.end())
	{
		mUpdateList.erase(iter);
	}

	// Terminate the game object
	gameObject->Terminate();

	// Next destroy the game object
	mGameObjectFactory->Destroy(gameObject);
}

void World::ProcessDestroyList()
{
	for (auto gameObject : mDestroyList)
	{
		DestroyInternal(gameObject);
	}
	mDestroyList.clear();
}

ResourceManager* World::GetResourceManager() const
{
	return mResourceManager.get();
}