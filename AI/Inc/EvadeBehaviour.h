#ifndef INCLUDED_AI_EVADEBEHAVIOUR_H
#define INCLUDED_AI_EVADEBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class EvadeBehaviour : public SteeringBehaviour
{
public:
	EvadeBehaviour();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Evade"; }
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_EVADEBEHAVIOUR_H