#ifndef INCLUDED_AI_SEEKBEHAVIOUR_H
#define INCLUDED_AI_SEEKBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI 
{

class SeekBehaviour : public SteeringBehaviour
{
public:
	SeekBehaviour();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Seek"; }
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_SEEKBEHAVIOUR_H