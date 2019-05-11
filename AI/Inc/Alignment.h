#ifndef INCLUDED_AI_ALIGNMENT_H
#define INCLUDED_AI_ALIGNMENT_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class Alignment : public SteeringBehaviour
{
public:
	Alignment();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Alignment"; }

private:
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_ALIGNMENT_H