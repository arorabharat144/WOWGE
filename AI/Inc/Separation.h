#ifndef INCLUDED_AI_SEPARATION_H
#define INCLUDED_AI_SEPARATION_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class Separation : public SteeringBehaviour
{
public:
	Separation();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Separation"; }



private:

};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_ARRIVEBEHAVIOUR_H