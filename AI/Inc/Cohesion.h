#ifndef INCLUDED_AI_COHESION_H
#define INCLUDED_AI_COHESION_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class Cohesion : public SteeringBehaviour
{
public:
	Cohesion();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Cohesion"; }

private:

};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_COHESION_H