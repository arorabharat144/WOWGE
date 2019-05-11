#include "Sit.h"

#include "WorldManager.h"

#include "StateTypes.h"

using namespace WOWGE;

void Sit::Enter(OfficeWorker& agent)
{
	mTimer = 0.0f;

	agent.SetCurrentAnimation("SittingIdle", true);

	agent.Position().x += WorldManager::Get()->CellSize();

	mWorkTimer = Maths::RandomFloat(5.0f, 10.0f);
}

void Sit::Update(OfficeWorker& agent, float deltaTime)
{
	if (WorldManager::Get()->HasTriggeredAlarm())
	{
		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>((StateTypes::SitToStand))));
		return;
	}

	mTimer += deltaTime;
	if (mTimer >= mWorkTimer)
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

		agent.GetCurrentDestination()->spotTaken = false;

		agent.GetCurrentDestination() = &WorldManager::Get()->Destinations()[destination];

		agent.GetPathPlanner().RequestPath({ xCoord, 0.0f, zCoord });

		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>((StateTypes::SitToStand))));
	}
}

void Sit::Exit(OfficeWorker& agent)
{
	
}