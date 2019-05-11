#include "Walk.h"

#include "WorldManager.h"

#include "StateTypes.h"

#include <Math/Inc/Maths.h>

using namespace WOWGE;

void Walk::Enter(OfficeWorker& agent)
{
	agent.SetCurrentAnimation("Walking", true);

	agent.MaxSpeed() = WorldManager::Get()->MaxAgentSpeed();

	agent.GetSteeringModule().GetBehaviour<WOWGE::AI::SeekBehaviour>("Seek")->SetActive(true);

	agent.GetPath() = agent.GetPathPlanner().GetPath();

	mDestination = { static_cast<float>(agent.GetPath().front().x), 0.0f, static_cast<float>(agent.GetPath().front().y) };
}

void Walk::Update(OfficeWorker& agent, float deltaTime)
{
	if (WorldManager::Get()->HasTriggeredAlarm())
	{
		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>((StateTypes::Evacuate))));
		return;
	}

	if (agent.GetPath().size() == 0)
	{
		if ((agent.GetCurrentDestination()->name.find("couch") != std::string::npos) ||
			agent.GetCurrentDestination()->name.find("computer") != std::string::npos)
		{
			agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>(StateTypes::StandToSit)));
			return;
		}
		else if (agent.GetCurrentDestination()->name.find("filing") != std::string::npos)
		{
			agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>(StateTypes::Filing)));
			return;
		}
	}
	if (Maths::DistanceSquare(
			{ agent.Position().x, agent.Position().z },
			{ static_cast<float>(agent.GetPath().back().x), static_cast<float>(agent.GetPath().back().y) }) >= (agent.Radius() * agent.Radius()))
	{
		agent.Destination() = { static_cast<float>(agent.GetPath().back().x), 0.0f, static_cast<float>(agent.GetPath().back().y) };
	}
	else
	{
		agent.GetPath().pop_back();
	}
}

void Walk::Exit(OfficeWorker& agent)
{
	if (!WorldManager::Get()->HasTriggeredAlarm())
	{
		agent.MaxSpeed() = 0.0f;
		agent.GetSteeringModule().GetBehaviour<WOWGE::AI::SeekBehaviour>("Seek")->SetActive(false);

		agent.Heading() = agent.GetCurrentDestination()->normal;

		if ((agent.GetCurrentDestination()->name.find("couch") != std::string::npos) ||
			(agent.GetCurrentDestination()->name.find("computer") != std::string::npos))
		{
			agent.Position() = { mDestination.x, 7.0f, mDestination.z };
		}
		else
		{
			agent.Position() = { mDestination.x, 0.0f, mDestination.z };
		}

		if ((agent.GetCurrentDestination()->name.find("filingCabinet3") != std::string::npos) ||
			(agent.GetCurrentDestination()->name.find("filingCabinet4") != std::string::npos))
		{
			agent.Position().z += WorldManager::Get()->CellSize();
		}
	}
}