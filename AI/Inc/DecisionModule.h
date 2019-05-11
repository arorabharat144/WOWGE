#ifndef INCLUDED_AI_DECISIONMODULE_H
#define INCLUDED_AI_DECISIONMODULE_H

#include "Goal.h"
#include "GoalComposite.h"
#include "Strategy.h"

namespace WOWGE
{
namespace AI
{

template <class AgentType>
class DecisionModule
{
public:
	using GoalType = Goal<AgentType>;
	using StrategyType = Strategy<AgentType>;

	DecisionModule(AgentType& agent);
	~DecisionModule();

	StrategyType* AddStrategy(StrategyType* strategy);
	void Purge();

	void Update();

private:
	void Arbitrate();

	AgentType& mAgent;
	std::vector<std::unique_ptr<StrategyType>> mStrategies;
	StrategyType* mCurrentStrategy;
	GoalType* mCurrentGoal;
};

template <class AgentType>
DecisionModule<AgentType>::DecisionModule(AgentType& agent)
	: mAgent(agent)
	, mCurrentStrategy(nullptr)
	, mCurrentGoal(nullptr)
{
}

template <class AgentType>
DecisionModule<AgentType>::~DecisionModule()
{
	Purge();
}

template <class AgentType>
typename DecisionModule<AgentType>::StrategyType* DecisionModule<AgentType>::AddStrategy(StrategyType* strategy)
{
	mStrategies.emplace_back(std::make_unique<StrategyType>());
	return static_cast<StrategyType*>(mStrategies.back().get());
}

template <class AgentType>
void DecisionModule<AgentType>::Purge()
{
	// Remove all strategies
	mStrategies.clear();

	// Remove current goal
	if (mCurrentGoal != nullptr)
	{
		mCurrentGoal->Terminate();
	}
}

template <class AgentType>
void DecisionModule<AgentType>::Update()
{
	// Evaluate our priorities
	Arbitrate();

	// Update the current action
	if (mCurrentGoal != nullptr)
	{
		mCurrentGoal->Process();
	}
}

template <class AgentType>
void DecisionModule<AgentType>::Arbitrate()
{
	float mostDesirable = 0.0f;
	StrategyType* bestStrategy = nullptr;

	// Find the most desirable strategy
	for (auto strategy : mStrategies)
	{
		float desirability = strategy->CalculateDesirability();
		if (desirability > mostDesirable)
		{
			mostDesirable = desirability;
			bestStrategy = strategy;
		}
	}

	// Check if our strategy has changed
	if (mCurrentStrategy != bestStrategy)
	{
		// Set new strategy
		mCurrentStrategy = bestStrategy;

		// Exit the current goal
		if (mCurrentGoal != nullptr)
		{
			mCurrentGoal->Terminate();
		}

		// Set new goal
		if (mCurrentStrategy != nullptr)
		{
			mCurrentGoal = mCurrentStrategy->CreateGoal();
		}
	}
}

} // namespace AI
} // namespace WOWGE

#endif // #ifndef INCLUDED_AI_DECISIONMODULE_H