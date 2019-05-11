#include "Precompiled.h"

#include "EvadeBehaviour.h"
#include "Agent.h"

using namespace WOWGE;
using namespace WOWGE::AI;

EvadeBehaviour::EvadeBehaviour()
{

}

Maths::Vector3 EvadeBehaviour::Calculate(Agent& agent)
{
	Maths::Vector3 distance{ agent.Destination() - agent.Position() };
	float updatesAhead{ Maths::Magnitude(distance) / agent.MaxSpeed() };

	Maths::Vector3 futurePosition{ agent.Destination() + agent.Velocity() * updatesAhead };

	Maths::Vector3 desiredVelocity{ Maths::Normalize(agent.Position() - futurePosition) * agent.MaxSpeed() };

	return desiredVelocity - agent.Velocity();
}