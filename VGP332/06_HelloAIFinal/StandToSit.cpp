#include "StandToSit.h"

#include "StateTypes.h"

void StandToSit::Enter(OfficeWorker& agent)
{
	mTimer = 0.0f;
	agent.SetCurrentAnimation("StandToSit", false);
}

void StandToSit::Update(OfficeWorker& agent, float deltaTime)
{
	mTimer += deltaTime;

	if (mTimer >= agent.GetCurrentAnimation()->duration / agent.GetCurrentAnimation()->ticksPerSecond)
	{
		if (agent.GetCurrentDestination()->name.find("couch") != std::string::npos)
		{
			agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>(StateTypes::Sit)));
		}
		else if (agent.GetCurrentDestination()->name.find("computer") != std::string::npos)
		{
			agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>(StateTypes::SitToType)));
		}
	}
}

void StandToSit::Exit(OfficeWorker& agent)
{

}