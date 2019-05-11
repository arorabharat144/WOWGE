#ifndef INCLUDED_AI_PURSUITBEHAVIOUR_H
#define INCLUDED_AI_PURSUITBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class PursuitBehaviour : public SteeringBehaviour
{
public:
	PursuitBehaviour();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Pursuit"; }
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_PURSUITBEHAVIOUR_H