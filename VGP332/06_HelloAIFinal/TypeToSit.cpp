#include "TypeToSit.h"

#include "StateTypes.h"

#include "WorldManager.h"

void TypeToSit::Enter(OfficeWorker& agent)
{
	mTimer = 0.0f;
	agent.SetCurrentAnimation("TypeToSit", false);
}

void TypeToSit::Update(OfficeWorker& agent, float deltaTime)
{
	mTimer += deltaTime;
	if (mTimer >= agent.GetCurrentAnimation()->duration / agent.GetCurrentAnimation()->ticksPerSecond)
	{
		if (WorldManager::Get()->HasTriggeredAlarm())
		{
			agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>((StateTypes::SitToStand))));
			return;
		}
		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>((StateTypes::SitToStand))));
	}

}

void TypeToSit::Exit(OfficeWorker& agent)
{

}