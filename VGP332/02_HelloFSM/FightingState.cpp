#include "FightingState.h"

#include "WorldManager.h"

void FightingState::Enter(IronManAgent& agent)
{
	WorldManager::Get()->EnemyHealth() = 100;

	agent.HasReachedMiningLocation() = false;
	agent.HasReachedEnemyLocation() = true;
	agent.HasReachedHomeLocation() = false;
}

void FightingState::Update(IronManAgent& agent, float deltaTime)
{
	mTimer += deltaTime;
	if (mTimer > 0.05f)
	{
		WorldManager::Get()->EnemyHealth() -= 1;

		mTimer = 0;
	}

	if (WorldManager::Get()->EnemyHealth() <= 0)
	{
		WorldManager::Get()->EnemyHealth() = 0;

		agent.StateMachine()->ChangeState(0);
	}
}

void FightingState::Exit(IronManAgent& agent)
{

}