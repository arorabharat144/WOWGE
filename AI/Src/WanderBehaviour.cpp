#include "Precompiled.h"

#include "WanderBehaviour.h"
#include "Agent.h"

using namespace WOWGE;
using namespace WOWGE::AI;

WanderBehaviour::WanderBehaviour()
{

}

Maths::Vector3 WanderBehaviour::Calculate(Agent& agent)
{
	mWanderTarget +=
	{
		Maths::RandomFloat(-1.0f, 1.0f) * mWanderJitter,
		0.0f,
		Maths::RandomFloat(-1.0f, 1.0f) * mWanderJitter
	};

	mWanderTarget = Maths::Normalize(mWanderTarget);
	mWanderTarget *= mCircleRadius;

	Maths::Vector3 target{ mWanderTarget + Maths::Vector3(mCircleDistance, 0.0f, mCircleDistance) };

	mTargetInWorldSpace = Maths::GetTranslation(Maths::Matrix44::Translation(agent.Position()) * Maths::Matrix44::Translation(target)) ;

	return mTargetInWorldSpace - agent.Position();
}