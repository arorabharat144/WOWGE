#include "Precompiled.h"
#include "ResourceManager.h"

#include "Resource.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

ResourceManager::ResourceManager()
{

}


ResourceManager::~ResourceManager()
{

}

void ResourceManager::Initialize()
{

}

void ResourceManager::Terminate()
{
	for (auto& resource : mResources)
	{
		resource.second->Terminate();
	}
}

Resource* ResourceManager::GetResource(std::string name)
{
	return mResources[name].get();
}