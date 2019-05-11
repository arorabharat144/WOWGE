#ifndef INCLUDED_AI_GOALCOMPOSITE_H
#define INCLUDED_AI_GOALCOMPOSITE_H

#include "Goal.h"

namespace WOWGE
{
namespace AI 
{

template <class AgentType>
class GoalComposite : public Goal<AgentType>
{
public:
	using GoalType = Goal<AgentType>;

	GoalComposite(AgentType& agent);
	virtual ~GoalComposite();

protected:
	void AddSubgoal(Goal<AgentType>* goal);
	void RemoveAllSubgoals();
	Goal<AgentType>::Status ProcessSubgoals();

	std::list<std::unique_ptr<GoalType>> mSubgoals;
};

template <class AgentType>
GoalComposite<AgentType>::GoalComposite(AgentType& agent)
	: Goal<AgentType>(agent)
{
}

template <class AgentType>
GoalComposite<AgentType>::~GoalComposite()
{
	RemoveAllSubgoals();
}

template <class AgentType>
void GoalComposite<AgentType>::AddSubgoal(Goal<AgentType>* goal)
{
	mSubgoals.push_front(goal);
}

template <class AgentType>
void GoalComposite<AgentType>::RemoveAllSubgoals()
{
	for (auto goal : mSubgoals)
	{
		if (goal != nullptr)
		{
			goal->Terminate();
		}
	}
	mSubgoals.clear();
}

template <class AgentType>
typename Goal<AgentType>::Status GoalComposite<AgentType>::ProcessSubgoals()
{
	// Remove all completed goals
	while (!mSubgoals.empty())
	{
		Goal<AgentType>* goal = mSubgoals.front();
		if (goal->mStatus != Status::Completed &&
			goal->mStatus != Status::Failed)
		{
			break;
		}

		goal->Terminate();
		mSubgoals.pop_front();
	}

	// If any Goals remain, continue to process
	if (!mSubgoals.empty())
	{
		typename Goal<AgentType>::Status status = mSubgoals.front()->Process();
		if (status == Status::Completed && mSubgoals.size() > 1)
		{
			return Status::Active;
		}
		else
		{
			return status;
		}
	}
	return Status::Completed;
}

} // namespace AI
} // namespace WOWGE

#endif // #ifndef INCLUDED_AI_GOALCOMPOSITE_H