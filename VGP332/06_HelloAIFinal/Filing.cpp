#include "Filing.h"

#include "WorldManager.h"

#include "StateTypes.h"

using namespace WOWGE;

void Filing::Enter(OfficeWorker& agent)
{
	mTimer = 0.0f;
	agent.SetCurrentAnimation("FilingCabinet", false);

	//agent.Position().x += WorldManager::Get()->CellSize();
}

void Filing::Update(OfficeWorker& agent, float deltaTime)
{
	if (WorldManager::Get()->HasTriggeredAlarm())
	{
		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>((StateTypes::Evacuate))));
		return;
	}

	mTimer += deltaTime;
	if (mTimer >= agent.GetCurrentAnimation()->duration / agent.GetCurrentAnimation()->ticksPerSecond)
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

		agent.GetStateMachine().ChangeState(static_cast<uint32_t>(static_cast<int>((StateTypes::Walk))));

		agent.GetPath() = agent.GetPathPlanner().GetPath();
		agent.GetPath().pop_back();
		agent.Destination() = { static_cast<float>(agent.GetPath().back().x), 0.0f, static_cast<float>(agent.GetPath().back().y) };
	}
}

void Filing::Exit(OfficeWorker& agent)
{

}