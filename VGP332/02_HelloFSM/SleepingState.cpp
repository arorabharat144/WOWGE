#include "SleepingState.h"

#include"WorldManager.h"

void SleepingState::Enter(IronManAgent& agent)
{
	WorldManager::Get()->RestPercent() = 0;
	WorldManager::Get()->IronReserves() = 0;
	WorldManager::Get()->EnemyHealth() = 100;

	mTimer = 0.0f;

	agent.HasReachedMiningLocation() = false;
	agent.HasReachedEnemyLocation() = false;
	agent.HasReachedHomeLocation() = true;
}

void SleepingState::Update(IronManAgent& agent, float deltaTime)
{
	mTimer += deltaTime;
	if (mTimer > 0.05f)
	{
		WorldManager::Get()->RestPercent() += 1;

		mTimer = 0;
	}

	if (WorldManager::Get()->RestPercent() >= 100)
	{
		WorldManager::Get()->RestPercent() = 100;

		agent.StateMachine()->ChangeState(0);
	}
}

void SleepingState::Exit(IronManAgent& agent)
{

}