#include "Precompiled.h"

#include "Alignment.h"
#include "Agent.h"

#include "AIWorld.h"

using namespace WOWGE;
using namespace WOWGE::AI;

Alignment::Alignment()
{

}

Maths::Vector3 Alignment::Calculate(Agent& agent)
{
	Maths::Vector3 averageHeading;

	AgentList neighbours{ agent.Neighbours() };

	for (uint32_t i{ 0 }; i < neighbours.size(); ++i)
	{
		averageHeading += neighbours[i]->Heading();
	}

	if (neighbours.size() > 0)
	{
		averageHeading /= static_cast<float>(neighbours.size());

		averageHeading -= agent.Heading();
	}
	return averageHeading;
}