#include "Precompiled.h"

#include "SeekBehaviour.h"
#include "Agent.h"

using namespace WOWGE;
using namespace WOWGE::AI;

SeekBehaviour::SeekBehaviour()
{

}

Maths::Vector3 SeekBehaviour::Calculate(Agent& agent)
{
	Maths::Vector3 desiredVelocity{ Maths::Normalize(agent.Destination() - agent.Position()) * agent.MaxSpeed() };

	return desiredVelocity - agent.Velocity();
}