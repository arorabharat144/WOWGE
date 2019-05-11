#ifndef INCLUDED_ENGINE_STATEMACHINE_H
#define INCLUDED_ENGINE_STATEMACHINE_H

#include "State.h"

namespace WOWGE
{
namespace Engine
{

template<class ClassType>
class StateMachine
{
public:
	StateMachine(ClassType& classRef);
	~StateMachine();

	template<class NewState>
	void AddState();

	void Purge();

	void Update(float deltaTime);
	void ChangeState(uint32_t index);

private:
	ClassType& mClassRef;
	State* mCurrentState;
	int mNextStateIndex;
	std::vector<std::unique_ptr<State>> mStates;
};

template<class ClassType>
StateMachine<ClassType>::StateMachine(ClassType& classRef) :
	mClassRef{ classRef },
	mCurrentState{ nullptr },
	mNextStateIndex{ -1 }
{

}

template<class ClassType>
StateMachine<ClassType>::~StateMachine()
{
	ASSERT(mStates.empty(), "[StateMachine] Purge must be called before destruction.");
}

template<class ClassType>
template<class NewState>
void StateMachine<ClassType>::AddState()
{
	static_assert(std::is_base_of_v<State, NewState>,
		"[StateMachine] Can only add state for State type");

	mStates.emplace_back(std::make_unique<NewState>());
}

template<class ClassType>
void StateMachine<ClassType>::Purge()
{
	mStates.clear();
}

template<class ClassType>
void StateMachine<ClassType>::Update(float deltaTime)
{
	ASSERT(mCurrentState != nullptr, "[StateMachine] mCurrentState is null!");
	mNextStateIndex = mCurrentState->Update(deltaTime);

	if (mNextStateIndex != -1)
	{
		ChangeState(mNextStateIndex);
	}
}

template<class ClassType>
void StateMachine<ClassType>::ChangeState(uint32_t index)
{
	ASSERT(index < static_cast<uint32_t>(mStates.size()),
		"[StateMachine] Invalid index %d", index);

	if (mCurrentState)
	{
		mCurrentState->Terminate();
	}
	mCurrentState = mStates[index].get();
	mCurrentState->Initialize();
}

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_STATEMACHINE_H
