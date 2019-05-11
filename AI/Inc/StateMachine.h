#ifndef INCLUDED_AI_STATEMACHINE_H
#define INCLUDED_AI_STATEMACHINE_H

namespace WOWGE
{
namespace AI
{

template<class AgentType>
class StateMachine
{
public:
	using StateType = State<AgentType>;

	StateMachine(AgentType& agent);
	~StateMachine();

	template<class NewStateType>
	void AddState();

	void Purge();

	void Update(float deltaTime);
	void ChangeState(uint32_t index);

private:
	AgentType& mAgent;
	StateType* mCurrentState;
	std::vector<std::unique_ptr<StateType>> mStates;
};

template<class AgentType>
StateMachine<AgentType>::StateMachine(AgentType& agent) :
	mAgent{ agent },
	mCurrentState{ nullptr }
{

}

template<class AgentType>
StateMachine<AgentType>::~StateMachine()
{
	ASSERT(mStates.empty(), "[StateMachine] Purge must be called before destruction.");
}

template<class AgentType>
template<class NewStateType>
void StateMachine<AgentType>::AddState()
{
	static_assert(std::is_base_of<StateType, NewStateType>::value,
		"[StateMachine] Can only add state for AgentType");
	mStates.emplace_back(std::make_unique<NewStateType>());
}

template<class AgentType>
void StateMachine<AgentType>::Purge()
{
	mStates.clear();
}

template<class AgentType>
void StateMachine<AgentType>::Update(float deltaTime)
{
	ASSERT(mCurrentState != nullptr, "[StateMachine] mCurrentState is null!");
	mCurrentState->Update(mAgent, deltaTime);
}

template<class AgentType>
void StateMachine<AgentType>::ChangeState(uint32_t index)
{
	ASSERT(index < static_cast<uint32_t>(mStates.size()), 
		"[StateMachine] Invalid index %d", index);

	if (mCurrentState)
	{
		mCurrentState->Exit(mAgent);
	}
	mCurrentState = mStates[index].get();
	mCurrentState->Enter(mAgent);
}

} //namespace AI
} //namespace WOWGE

#endif // !INCLUDED_AI_STATEMACHINE_H
