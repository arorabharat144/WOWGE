#include "Huddle.h"

#include "WorldManager.h"

#include <Math/Inc/Maths.h>

#include "StateTypes.h"

using namespace WOWGE;

namespace
{
	uint32_t spot{ 2 };
}

void Huddle::Enter(OfficeWorker& agent)
{
	agent.MaxSpeed() = 0.0f;

	if (agent.GetOfficeWorkerIndex() == WorldManager::Get()->BossIndex())
	{
		agent.SetCurrentAnimation("Talking", true);
		for (uint32_t i{ 0 }; i < WorldManager::Get()->Destinations().size(); ++i)
		{
			if (WorldManager::Get()->Destinations()[i].name.compare("huddle1") == 0)
			{
				agent.Position().x = 2.0f * WorldManager::Get()->Destinations()[i].coordinate.x * WorldManager::Get()->CellSize();
				agent.Position().z = 2.0f * WorldManager::Get()->Destinations()[i].coordinate.y * WorldManager::Get()->CellSize();

				agent.Heading() = { 0.0f, 0.0f, -1.0f };

				WorldManager::Get()->Destinations()[i].spotTaken = true;

				WorldManager::Get()->Boss() = &agent;
				break;
			}
		}
	}
	else
	{
		agent.SetCurrentAnimation("Idle", true);
		for (uint32_t i{ 0 }; i < WorldManager::Get()->Destinations().size(); ++i)
		{
			std::string subString{ WorldManager::Get()->Destinations()[i].name.substr(0, 6) };
			if (subString.compare("huddle") == 0 && WorldManager::Get()->Destinations()[i].name.compare("huddle1") != 0)
			{
				if (static_cast<int>(WorldManager::Get()->Destinations()[i].name.back() - '0') == spot)
				{
					agent.Position().x = 2.0f * WorldManager::Get()->Destinations()[i].coordinate.x * WorldManager::Get()->CellSize();
					agent.Position().z = 2.0f * WorldManager::Get()->Destinations()[i].coordinate.y * WorldManager::Get()->CellSize();

					WorldManager::Get()->Destinations()[i].spotTaken = true;

					spot++;

					return;
				}
			}
		}
		
	}
}

void Huddle::Update(OfficeWorker& agent, float deltaTime)
{
	if (WorldManager::Get()->HasStartedSimulation())
	{
		uint32_t destination{ 0 };
		std::string str;
		do
		{
			destination = static_cast<uint32_t>(Maths::RandomInt(0, static_cast<int>(WorldManager::Get()->Destinations().size() - 1)));
			str = WorldManager::Get()->Destinations()[destination].name;
		} while (str.find("huddle") != std::string::npos || str.find("exit") != std::string::npos || WorldManager::Get()->Destinations()[destination].spotTaken);

		float xCoord{ 2.0f * WorldManager::Get()->Destinations()[destination].coordinate.x * WorldManager::Get()->CellSize() };
		float zCoord{ 2.0f * WorldManager::Get()->Destinations()[destination].coordinate.y * WorldManager::Get()->CellSize() };

		WorldManager::Get()->Destinations()[destination].spotTaken = true;

		agent.GetCurrentDestination() = &WorldManager::Get()->Destinations()[destination];

		agent.GetPathPlanner().RequestPath({ xCoord, 0.0f, zCoord });

		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>((StateTypes::Walk))));
	}

	//Face the boss!
	if (agent.GetOfficeWorkerIndex() != WorldManager::Get()->BossIndex())
	{
		agent.Heading() = Maths::Normalize(WorldManager::Get()->Boss()->Position() - agent.Position());
	}
}

void Huddle::Exit(OfficeWorker& agent)
{
	for (uint32_t i{ 0 }; i < WorldManager::Get()->Destinations().size(); ++i)
	{
		std::string subString{ WorldManager::Get()->Destinations()[i].name.substr(0, 6) };

		if (subString.compare("huddle") == 0)
		{
			WorldManager::Get()->Destinations()[i].spotTaken = false;
		}
	}
}