#ifndef INCLUDED_AI_OBSTACLEAVOIDANCEBEHAVIOUR_H
#define INCLUDED_AI_OBSTACLEAVOIDANCEBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class ObstacleAvoidanceBehaviour : public SteeringBehaviour
{
public:
	ObstacleAvoidanceBehaviour();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "ObstacleAvoidance"; }

	float& GetMaxAvoidForce() { return mMaxAvoidForce; }

	Maths::Vector3 ahead;
	Maths::Vector3 ahead2;

private:
	float mMaxAvoidForce{ 0.0f };

	float mDetectionBoxLength{ 0.0f };
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_OBSTACLEAVOIDANCEBEHAVIOUR_H