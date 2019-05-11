#ifndef INCLUDED_AI_STEERINGMODULE_H
#define INCLUDED_AI_STEERINGMODULE_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class SteeringModule
{
public:
	SteeringModule(Agent& agent);
	~SteeringModule();

	template <class BehaviourType>
	BehaviourType* AddBehaviour();
	template <class BehaviourType>
	BehaviourType* GetBehaviour(const char* name);

	void Purge();
	Maths::Vector3 Calculate();

private:
	Agent& mAgent;
	std::vector<std::unique_ptr<SteeringBehaviour>> mBehaviours;
};

template <class BehaviourType>
BehaviourType* SteeringModule::AddBehaviour()
{
	mBehaviours.push_back(std::make_unique<BehaviourType>());
	return static_cast<BehaviourType*>(mBehaviours.back().get());
}

template <class BehaviourType>
BehaviourType* SteeringModule::GetBehaviour(const char* name)
{
	for (auto& b : mBehaviours)
	{
		if (strcmp(b->GetName(), name) == 0)
		{
			return static_cast<BehaviourType*>(b.get());
		}
	}

	return nullptr;
}

} //namespace AI
} //namespace WOWGE

#endif // !INCLUDED_AI_STEERINGMODULE_H
