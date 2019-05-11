#include "Precompiled.h"

#include "Separation.h"
#include "Agent.h"

#include "AIWorld.h"

using namespace WOWGE;
using namespace WOWGE::AI;

Separation::Separation()
{

}

Maths::Vector3 Separation::Calculate(Agent& agent)
{
	Maths::Vector3 steeringForce;
	AgentList neighbours{ agent.Neighbours() };

	for (uint32_t i{ 0 }; i < neighbours.size(); ++i)
	{
		Maths::Vector3 toAgent{ agent.Position() - neighbours[i]->Position() };

		steeringForce += Maths::Normalize(toAgent) / Maths::Magnitude(toAgent);
	}

	return steeringForce;
}