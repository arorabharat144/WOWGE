#include "IronManAgent.h"

#include "TraversalState.h"
#include "MiningState.h"
#include "FightingState.h"
#include "SleepingState.h"

IronManAgent::IronManAgent()
{

}

IronManAgent::~IronManAgent()
{

}

void IronManAgent::Initialize()
{
	mStateMachine = std::make_unique<WOWGE::AI::StateMachine<IronManAgent>>(*this);

	mStateMachine->AddState<TraversalState>();
	mStateMachine->AddState<MiningState>();
	mStateMachine->AddState<FightingState>();
	mStateMachine->AddState<SleepingState>();

	mStateMachine->ChangeState(3);
}

void IronManAgent::Terminate()
{
	mStateMachine->Purge();
}

void IronManAgent::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
}