#include "SitToStand.h"

#include "StateTypes.h"

#include "WorldManager.h"

void SitToStand::Enter(OfficeWorker& agent)
{
	mTimer = 0.0f;
	agent.SetCurrentAnimation("SitToStand", false);

	agent.Position().y = 0.0f;
}

void SitToStand::Update(OfficeWorker& agent, float deltaTime)
{
	mTimer += deltaTime;

	if (mTimer >= agent.GetCurrentAnimation()->duration / agent.GetCurrentAnimation()->ticksPerSecond)
	{
		if (WorldManager::Get()->HasTriggeredAlarm())
		{
			agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>((StateTypes::Evacuate))));
			return;
		}

		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>(StateTypes::Walk)));
	}
}

void SitToStand::Exit(OfficeWorker& agent)
{

}