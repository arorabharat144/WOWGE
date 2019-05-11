#ifndef INCLUDED_AI_FLEEBEHAVIOUR_H
#define INCLUDED_AI_FLEEBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class FleeBehaviour : public SteeringBehaviour
{
public:
	FleeBehaviour();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Flee"; }
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_FLEEBEHAVIOUR_H