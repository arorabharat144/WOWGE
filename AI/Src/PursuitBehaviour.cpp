#include "Precompiled.h"

#include "PursuitBehaviour.h"
#include "Agent.h"

using namespace WOWGE;
using namespace WOWGE::AI;

PursuitBehaviour::PursuitBehaviour()
{

}

Maths::Vector3 PursuitBehaviour::Calculate(Agent& agent)
{
	Maths::Vector3 distance{ agent.Destination() - agent.Position() };
	float t{ Maths::Magnitude(distance) / agent.MaxSpeed() };

	Maths::Vector3 futurePosition{ agent.Destination() + agent.Velocity() * t };

	Maths::Vector3 desiredVelocity{ Maths::Normalize(futurePosition - agent.Position()) * agent.MaxSpeed() };

	return desiredVelocity - agent.Velocity();
}