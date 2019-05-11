#include "TraversalState.h"

#include "WorldManager.h"

void TraversalState::Enter(IronManAgent& agent)
{
	mStartPosition = agent.Position();
	mTimer = 0.0f;

	mStartRotation = agent.Rotation();

	if (agent.HasReachedHomeLocation())
	{
		mDestination = WorldManager::Get()->MiningLocation();

		mEndRotation = WOWGE::Maths::ToQuaternion({ WOWGE::Maths::ConvertDegreeToRadian(-90.0f), WOWGE::Maths::ConvertDegreeToRadian(180.0f), WOWGE::Maths::ConvertDegreeToRadian(0.0f) });
	}
	else if (agent.HasReachedMiningLocation())
	{
		mDestination = WorldManager::Get()->EnemyLocation();

		mEndRotation = WOWGE::Maths::ToQuaternion({ WOWGE::Maths::ConvertDegreeToRadian(-90.0f), WOWGE::Maths::ConvertDegreeToRadian(-90.0f), WOWGE::Maths::ConvertDegreeToRadian(0.0f) });
	}
	else if (agent.HasReachedEnemyLocation())
	{
		mDestination = WorldManager::Get()->HomeLocation();

		mEndRotation = WOWGE::Maths::ToQuaternion({ WOWGE::Maths::ConvertDegreeToRadian(-90.0f), WOWGE::Maths::ConvertDegreeToRadian(75.0f), WOWGE::Maths::ConvertDegreeToRadian(0.0f) });
	}

}

void TraversalState::Update(IronManAgent& agent, float deltaTime)
{
	mTimer += deltaTime;
	agent.Position() = WOWGE::Maths::Lerp(mStartPosition, mDestination, mTimer);
	agent.Position().y = mStartPosition.y;

	agent.Rotation() = WOWGE::Maths::Slerp(mStartRotation, mEndRotation, mTimer);

	if ((agent.Position().z >= WorldManager::Get()->MiningLocation().z) &&
		agent.HasReachedHomeLocation())
	{
		agent.Position() = WorldManager::Get()->MiningLocation();
		agent.Position().y = mStartPosition.y;

		agent.StateMachine()->ChangeState(1);
	}
	else if ((agent.Position().x >= WorldManager::Get()->EnemyLocation().x) &&
		agent.HasReachedMiningLocation())
	{
		agent.Position() = WorldManager::Get()->EnemyLocation();
		agent.Position().y = mStartPosition.y;

		agent.StateMachine()->ChangeState(2);
	}
	else if ((agent.Position().x <= WorldManager::Get()->HomeLocation().x) &&
		agent.HasReachedEnemyLocation())
	{
		agent.Position() = WorldManager::Get()->HomeLocation();
		agent.Position().y = mStartPosition.y;

		agent.StateMachine()->ChangeState(3);
	}
}

void TraversalState::Exit(IronManAgent& agent)
{

}