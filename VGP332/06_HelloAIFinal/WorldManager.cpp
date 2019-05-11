#include "WorldManager.h"

using namespace WOWGE;

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
	mBossIndex = static_cast<uint32_t>(Maths::RandomInt(0, 4));
}

void WorldManager::Update(float deltaTime)
{

}