#include "Precompiled.h"

#include "FleeBehaviour.h"
#include "Agent.h"

using namespace WOWGE;
using namespace WOWGE::AI;

FleeBehaviour::FleeBehaviour()
{

}

Maths::Vector3 FleeBehaviour::Calculate(Agent& agent)
{
	const float panicDistanceSquared{ 100.0f * 100.0f };
	if (Maths::DistanceSquare(agent.Position(), agent.Destination()) > panicDistanceSquared)
	{
		return { 0.0f, 0.0f, 0.0f };
	}

	Maths::Vector3 desiredVelocity{ Maths::Normalize(agent.Position() - agent.Destination()) * agent.MaxSpeed() };

	return desiredVelocity - agent.Velocity();
}