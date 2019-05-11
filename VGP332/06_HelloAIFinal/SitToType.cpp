#include "SitToType.h"

#include "WorldManager.h"
#include "StateTypes.h"

void SitToType::Enter(OfficeWorker& agent)
{
	mTimer = 0.0f;
	agent.SetCurrentAnimation("SitToType", false);

	if (agent.GetCurrentDestination()->name.compare("computer2"))
	{
		agent.Position().x -= WorldManager::Get()->CellSize() * 0.5f;
	}
	else if (agent.GetCurrentDestination()->name.compare("computer1"))
	{
		agent.Position().z -= WorldManager::Get()->CellSize() * 0.5f;
	}
}

void SitToType::Update(OfficeWorker& agent, float deltaTime)
{
	mTimer += deltaTime;

	if (mTimer >= agent.GetCurrentAnimation()->duration / agent.GetCurrentAnimation()->ticksPerSecond)
	{
		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>(StateTypes::Type)));
	}
}

void SitToType::Exit(OfficeWorker& agent)
{

}