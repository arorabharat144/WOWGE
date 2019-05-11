#ifndef INCLUDED_AI_WANDERBEHAVIOUR_H
#define INCLUDED_AI_WANDERBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class WanderBehaviour : public SteeringBehaviour
{
public:
	WanderBehaviour();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Wander"; }

	float& GetCircleRadius() { return mCircleRadius; }
	float& GetCircleDistance() { return mCircleDistance; }
	float& GetWanderJitter() { return mWanderJitter; }

	Maths::Vector3 GetTargetToFollowInWorldSpace() const { return mTargetInWorldSpace; }

private:
	float mCircleRadius{ 100.0f };
	float mCircleDistance{ 100.0f };
	float mWanderJitter{ 1.0f };

	Maths::Vector3 mWanderTarget;
	Maths::Vector3 mTargetInWorldSpace;
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_SEEKBEHAVIOUR_H