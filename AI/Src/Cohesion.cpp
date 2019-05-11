#include "Precompiled.h"

#include "Cohesion.h"
#include "Agent.h"

#include "AIWorld.h"

using namespace WOWGE;
using namespace WOWGE::AI;

Cohesion::Cohesion()
{

}

Maths::Vector3 Cohesion::Calculate(Agent& agent)
{
	Maths::Vector3 centreOfMass;
	Maths::Vector3 steeringForce;

	AgentList neighbours{ agent.Neighbours() };

	for (uint32_t i{ 0 }; i < neighbours.size(); ++i)
	{
		centreOfMass += neighbours[i]->Position();
	}

	if (neighbours.size() > 0)
	{
		centreOfMass /= static_cast<float>(neighbours.size());

		Maths::Vector3 desiredVelocity{ Maths::Normalize(centreOfMass - agent.Position()) * agent.MaxSpeed() };

		steeringForce = desiredVelocity - agent.Velocity();
	}

	return steeringForce;
}