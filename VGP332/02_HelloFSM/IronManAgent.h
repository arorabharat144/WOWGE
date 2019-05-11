#ifndef INCLUDED_IRONMANAGENT_H
#define INCLUDED_IRONMANAGENT_H

#include <AI/Inc/AI.h>

class IronManAgent
{
public:
	IronManAgent();
	~IronManAgent();

	void Initialize();
	void Terminate();

	void Update(float deltaTime);

	std::unique_ptr<WOWGE::AI::StateMachine<IronManAgent>>& StateMachine() { return mStateMachine; }

	WOWGE::Maths::Vector3& Position() { return mPosition; }
	const WOWGE::Maths::Vector3& Position() const { return mPosition; }

	WOWGE::Maths::Quaternion& Rotation() { return mRotation; }
	const WOWGE::Maths::Quaternion& Rotation() const { return mRotation; }

	bool& HasReachedMiningLocation() { return mHasReachedMiningLocation; }
	const bool& HasReachedMiningLocation() const { return mHasReachedMiningLocation; }

	bool& HasReachedEnemyLocation() { return mHasReachedEnemyLocation; }
	const bool& HasReachedEnemyLocation() const { return mHasReachedEnemyLocation; }

	bool& HasReachedHomeLocation() { return mHasReachedHomeLocation; }
	const bool& HasReachedHomeLocation() const { return mHasReachedHomeLocation; }

	float& Speed() { return mSpeed; }
	const float& Speed() const { return mSpeed; }

private:
	std::unique_ptr<WOWGE::AI::StateMachine<IronManAgent>> mStateMachine;

	WOWGE::Maths::Vector3 mPosition;
	WOWGE::Maths::Quaternion mRotation;
	
	bool mHasReachedMiningLocation{ false };
	bool mHasReachedEnemyLocation{ false };
	bool mHasReachedHomeLocation{ true };

	float mSpeed{ 20.0f };
};

#endif // !INCLUDED_IRONMANAGENT_H
