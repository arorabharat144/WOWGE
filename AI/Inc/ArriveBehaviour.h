#ifndef INCLUDED_AI_ARRIVEBEHAVIOUR_H
#define INCLUDED_AI_ARRIVEBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class ArriveBehaviour : public SteeringBehaviour
{
public:
	ArriveBehaviour();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Arrive"; }

	float& GetSlowingRadius() { return mSlowingRadius; }

private:
	float mSlowingRadius{ 1.0f };
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_ARRIVEBEHAVIOUR_H