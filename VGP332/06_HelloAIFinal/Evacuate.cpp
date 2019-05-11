#include "Evacuate.h"

#include "WorldManager.h"

#include "StateTypes.h"

using namespace WOWGE;

void Evacuate::Enter(OfficeWorker& agent)
{
	agent.SetCurrentAnimation("Running", true);

	agent.MaxSpeed() = 100.0f;

	for (uint32_t i{ 0 }; i < WorldManager::Get()->Destinations().size(); ++i)
	{
		if (WorldManager::Get()->Destinations()[i].name.find("exit") != std::string::npos)
		{
			if (!WorldManager::Get()->Destinations()[i].spotTaken)
			{
				float xCoord{ 2.0f * WorldManager::Get()->Destinations()[i].coordinate.x * WorldManager::Get()->CellSize() };
				float zCoord{ 2.0f * WorldManager::Get()->Destinations()[i].coordinate.y * WorldManager::Get()->CellSize() };

				agent.GetCurrentDestination() = &WorldManager::Get()->Destinations()[i];

				WorldManager::Get()->Destinations()[i].spotTaken = true;

				agent.GetPathPlanner().RequestPath({ xCoord, 0.0f, zCoord });
				break;
			}
		}
	}

	agent.GetSteeringModule().GetBehaviour<WOWGE::AI::SeekBehaviour>("Seek")->SetActive(true);

	agent.GetPath() = agent.GetPathPlanner().GetPath();
	agent.GetPath().pop_back();
	agent.Destination() = { static_cast<float>(agent.GetPath().back().x), 0.0f, static_cast<float>(agent.GetPath().back().y) };

	mDestination = { static_cast<float>(agent.GetPath().front().x), 0.0f, static_cast<float>(agent.GetPath().front().y) };
}

void Evacuate::Update(OfficeWorker& agent, float deltaTime)
{
	if (agent.GetPath().size() == 0)
	{
		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>(StateTypes::Idle)));
		return;
	}

	if (Maths::DistanceSquare(
		{ agent.Position().x, agent.Position().z },
		{ static_cast<float>(agent.GetPath().back().x), static_cast<float>(agent.GetPath().back().y) }) >= 2.0f * (agent.Radius() * agent.Radius()))
	{
		agent.Destination() = { static_cast<float>(agent.GetPath().back().x), 0.0f, static_cast<float>(agent.GetPath().back().y) };
	}
	else
	{
		agent.GetPath().pop_back();
	}
}

void Evacuate::Exit(OfficeWorker& agent)
{
	agent.MaxSpeed() = 0.0f;
	agent.GetSteeringModule().GetBehaviour<WOWGE::AI::SeekBehaviour>("Seek")->SetActive(false);

	agent.Position() = { mDestination.x, 0.0f, mDestination.z };
}