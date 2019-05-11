#include "MiningState.h"

#include "WorldManager.h"

void MiningState::Enter(IronManAgent& agent)
{
	WorldManager::Get()->IronReserves() = 0;

	agent.HasReachedMiningLocation() = true;
	agent.HasReachedEnemyLocation() = false;
	agent.HasReachedHomeLocation() = false;
}

void MiningState::Update(IronManAgent& agent, float deltaTime)
{
	mTimer += deltaTime;
	if (mTimer > 0.05f)
	{
		WorldManager::Get()->IronReserves() += 1;

		mTimer = 0;
	}

	if (WorldManager::Get()->IronReserves() >= 100)
	{
		WorldManager::Get()->IronReserves() = 100;

		agent.StateMachine()->ChangeState(0);
	}
}

void MiningState::Exit(IronManAgent& agent)
{

}