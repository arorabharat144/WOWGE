#include "Precompiled.h"

#include "ArriveBehaviour.h"
#include "Agent.h"

using namespace WOWGE;
using namespace WOWGE::AI;

ArriveBehaviour::ArriveBehaviour()
{

}

Maths::Vector3 ArriveBehaviour::Calculate(Agent& agent)
{
	Maths::Vector3 desiredVelocity{ agent.Destination() - agent.Position() };
	float distance{ Maths::Magnitude(desiredVelocity) };

	desiredVelocity = Maths::Normalize(desiredVelocity);

	if (distance > 1.0f)
	{
		if (distance < mSlowingRadius)
		{
			float speed{ (agent.MaxSpeed() / mSlowingRadius) * distance };
			desiredVelocity *= speed;
		}
		else
		{
			desiredVelocity *= agent.MaxSpeed();
		}

		return desiredVelocity - agent.Velocity();
	}

	agent.Velocity() = { 0.0f, 0.0f, 0.0f };
	return { 0.0f, 0.0f, 0.0f };
}