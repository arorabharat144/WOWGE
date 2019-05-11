#include "WorldManager.h"

namespace
{
	std::shared_ptr<WorldManager> sWorldManager{ nullptr };
}

void WorldManager::StaticInitialize()
{
	sWorldManager = std::make_shared<WorldManager>();
}

std::shared_ptr<WorldManager> WorldManager::Get()
{
	ASSERT(sWorldManager != nullptr, "[WorldManager] Must call StaticInitialize() first.");
	return sWorldManager;
}

WorldManager::WorldManager()
{

}

WorldManager::~WorldManager()
{

}

void WorldManager::Initialize()
{

}

void WorldManager::Terminate()
{

}

void WorldManager::Update(float deltaTime)
{

}