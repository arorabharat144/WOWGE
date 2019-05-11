#include "Ship.h"

#include <AI/Inc/ArriveBehaviour.h>
#include <AI/Inc/EvadeBehaviour.h>
#include <AI/Inc/FleeBehaviour.h>
#include <AI/Inc/ObstacleAvoidanceBehaviour.h>
#include <AI/Inc/PursuitBehaviour.h>
#include <AI/Inc/SeekBehaviour.h>
#include <AI/Inc/WallAvoidanceBehaviour.h>
#include <AI/Inc/WanderBehaviour.h>

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

Ship::Ship(AIWorld& aiWorld) :
	Agent{ aiWorld },
	mSteeringModule{ *this }
{

}

Ship::~Ship()
{
	mSteeringModule.Purge();
}

void Ship::Initialize(const WOWGE::Maths::Vector3& position, float maxSpeed, float mass)
{
	mPosition = position;
	mMaxSpeed = maxSpeed;
	mMass = mass;

	mSteeringModule.AddBehaviour<SeekBehaviour>();
	mSteeringModule.AddBehaviour<FleeBehaviour>();
	mSteeringModule.AddBehaviour<ArriveBehaviour>();
	mSteeringModule.AddBehaviour<PursuitBehaviour>();
	mSteeringModule.AddBehaviour<EvadeBehaviour>();
	mSteeringModule.AddBehaviour<WanderBehaviour>();
	mSteeringModule.AddBehaviour<ObstacleAvoidanceBehaviour>();
	mSteeringModule.AddBehaviour<WallAvoidanceBehaviour>();
}

void Ship::Update(float deltaTime)
{
	if (mSteeringModule.GetBehaviour<ObstacleAvoidanceBehaviour>("ObstacleAvoidance")->IsActive())
	{
		CalculateObstaclesInRange({ {mPosition.x, mPosition.z}, mRadius });
	}
	Maths::Vector3 steeringForce{ mSteeringModule.Calculate() };
	Maths::Vector3 acceleration{ steeringForce / mMass };

	mVelocity += acceleration * deltaTime;

	//Truncate velocity here
	mVelocity = Truncate(mVelocity, mMaxSpeed);

	mPosition.x += mVelocity.x * deltaTime;
	mPosition.z += mVelocity.z * deltaTime;

	if (Maths::MagnitudeSquare(mVelocity) > 0.0f)
	{
		Maths::Vector3 normalizedVelocity{ Maths::Normalize(mVelocity) };

		mHeading.x = normalizedVelocity.x;
		mHeading.z = normalizedVelocity.z;
	}
}

WOWGE::AI::SteeringModule& Ship::GetSteeringModule()
{
	return mSteeringModule;
}