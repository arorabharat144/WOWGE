#include "Precompiled.h"
#include "SteeringModule.h"

using namespace WOWGE;
using namespace WOWGE::AI;

SteeringModule::SteeringModule(Agent& agent) :
	mAgent{ agent }
{

}

SteeringModule::~SteeringModule()
{
	ASSERT(mBehaviours.empty(), "[SteeringModule] ");
}

void SteeringModule::Purge()
{
	mBehaviours.clear();
}

Maths::Vector3 SteeringModule::Calculate()
{
	Maths::Vector3 total;
	for (auto& b : mBehaviours)
	{
		if (b->IsActive())
		{
			total += b->Calculate(mAgent);
		}
	}

	return total;
}