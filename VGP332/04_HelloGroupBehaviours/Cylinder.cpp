#include "Cylinder.h"

#include "AI/Inc/AIWorld.h"

#include <AI/Inc/ObstacleAvoidanceBehaviour.h>
#include <AI/Inc/SeekBehaviour.h>
#include <AI/Inc/WanderBehaviour.h>
#include <AI/Inc/Separation.h>
#include <AI/Inc/Alignment.h>
#include <AI/Inc/Cohesion.h>

using namespace WOWGE;
using namespace WOWGE::AI;

namespace
{
	Maths::Vector3 Truncate(const Maths::Vector3& vector, float max)
	{
		Maths::Vector3 returnVector{ vector };
		float i{ max / Maths::Magnitude(returnVector) };
		if (i > 1.0f)
		{
			i = 1.0f;
		}

		returnVector *= i;

		return returnVector;
	}
}

Cylinder::Cylinder(AIWorld& aiWorld) :
	Agent{ aiWorld },
	mSteeringModule{ *this }
{

}

Cylinder::~Cylinder()
{
	mSteeringModule.Purge();
}

void Cylinder::Initialize(const WOWGE::Maths::Vector3& position, float maxSpeed, float mass)
{
	mPosition = position; 
	mMaxSpeed = maxSpeed;
	mMass = mass;

	mSteeringModule.AddBehaviour<Separation>();
	mSteeringModule.AddBehaviour<Alignment>();
	mSteeringModule.AddBehaviour<Cohesion>();
	mSteeringModule.AddBehaviour<WanderBehaviour>();
	mSteeringModule.AddBehaviour<SeekBehaviour>();
	//mSteeringModule.AddBehaviour<ObstacleAvoidanceBehaviour>();
	//mSteeringModule.AddBehaviour<FleeBehaviour>();
	//mSteeringModule.AddBehaviour<ArriveBehaviour>();
	//mSteeringModule.AddBehaviour<PursuitBehaviour>();
	//mSteeringModule.AddBehaviour<EvadeBehaviour>();
	//mSteeringModule.AddBehaviour<WanderBehaviour>();
	//mSteeringModule.AddBehaviour<ObstacleAvoidanceBehaviour>();
	//mSteeringModule.AddBehaviour<WallAvoidanceBehaviour>();
}

void Cylinder::Update(float deltaTime)
{
	mTimer += deltaTime;
	if (mTimer >= 1.0f)
	{
		if (mSteeringModule.GetBehaviour<Separation>("Separation")->IsActive() ||
			mSteeringModule.GetBehaviour<Alignment>("Alignment")->IsActive() ||
			mSteeringModule.GetBehaviour<Cohesion>("Cohesion")->IsActive())
		{
			CalculateNeighbourhood({ { mPosition.x, mPosition.z }, mRadius });
		}
		mTimer = 0.0f;
	}
	
	Maths::Vector3 steeringForce{ mSteeringModule.Calculate() };
	Maths::Vector3 acceleration{ steeringForce / mMass };

	mVelocity += acceleration * deltaTime;

	//Truncate velocity here
	mVelocity = Truncate(mVelocity, mMaxSpeed);

	mPosition.x += mVelocity.x * deltaTime;
	mPosition.z += mVelocity.z * deltaTime;

	EnforceNonPenetrationConstraint();

	if (Maths::MagnitudeXZSquare(mVelocity) > 0.0f)
	{
		mHeading = Maths::Normalize({mVelocity.x, 0.0f, mVelocity.z});
	}
}

WOWGE::AI::SteeringModule& Cylinder::GetSteeringModule()
{
	return mSteeringModule;
}