#include "Precompiled.h"
#include "Agent.h"

#include "AIWorld.h"

using namespace WOWGE;
using namespace WOWGE::AI;

Agent::Agent(AIWorld& world)
	: mWorld{ world }
	, mPosition{ 0.0f, 0.0f, 0.0f }
	, mVelocity{ 0.0f, 0.0f, 0.0f }
	, mDestination{ 0.0f, 0.0f, 0.0f }
	, mHeading{ 0.0f, 0.0f, 1.0f }
	, mMaxSpeed{ 0.0f}
{
	mWorld.RegisterAgent(this);
}

Agent::~Agent()
{
	mWorld.UnregisterAgent(this);
}

Maths::Matrix44 Agent::LocalToWorld()
{
	Maths::Vector3 up{ 0.0f, 1.0f, 0.0f };

	mSide = Maths::Cross(up, mHeading);

	return Maths::Matrix44
	{
		mSide.x, mSide.y, mSide.z, 1.0f,
		up.x,        up.y,       up.z,         1.0f,
		mHeading.x , mHeading.y , mHeading.z , 1.0f,
		mPosition.x, mPosition.y, mPosition.z, 1.0f
	};

	/*
	return X::Math::Matrix3(
	-mHeading.y, mHeading.x, 0.0f,
	mHeading.x, mHeading.y, 0.0f,
	mPosition.x, mPosition.y, 1.0f
	);
	*/
}

std::vector<Agent*> Agent::CalculateNeighbourhood(const Maths::Circle& range)
{
	mNeighbours.clear();
	std::vector<Agent*> allAgents{ mWorld.GetAllAgents() };

	for (uint32_t i{ 0 }; i < allAgents.size(); ++i)
	{
		Maths::Vector3 to{ allAgents[i]->mPosition - mPosition };

		float r{ range.radius + allAgents[i]->mRadius };

		if ((allAgents[i]->mPosition != mPosition) && (Maths::MagnitudeSquare(to) < r * r))
		{
			mNeighbours.emplace_back(allAgents[i]);
		}
	}
	return mNeighbours;
}

std::vector<Maths::Sphere> Agent::CalculateObstaclesInRange(const Maths::Circle& range)
{
	mObstacles.clear();
	std::vector<Maths::Sphere> allObstacles{ mWorld.GetObstacles() };

	for (uint32_t i{ 0 }; i < allObstacles.size(); ++i)
	{
		Maths::Vector3 to{ allObstacles[i].center - mPosition };

		float r{ range.radius + allObstacles[i].radius };

		if (Maths::MagnitudeSquare(to) < r * r)
		{
			mObstacles.emplace_back(allObstacles[i]);
		}
	}

	return mObstacles;
}

std::vector<Maths::AABB> Agent::CalculateWallsInRange(const Maths::Circle& range)
{
	mWalls.clear();
	std::vector<Maths::AABB> allWalls{ mWorld.GetWalls() };

	for (uint32_t i{ 0 }; i < allWalls.size(); ++i)
	{
		Maths::Vector3 to{ allWalls[i].center - mPosition };

		if (Maths::Intersect(Maths::Sphere({ range.center.x, mPosition.y, range.center.y }, range.radius), allWalls[i]))
		{
			mWalls.emplace_back(allWalls[i]);
		}

	}

	return mWalls;
}

void Agent::EnforceNonPenetrationConstraint()
{
	std::vector<Agent*> allAgents{ mWorld.GetAllAgents() };

	for (uint32_t i{ 0 }; i < allAgents.size(); ++i)
	{
		if (allAgents[i]->mPosition == mPosition)
		{
			continue;
		}

		Maths::Vector3 toAgent{ mPosition - allAgents[i]->mPosition };

		float distanceFromEachOther{ Maths::Magnitude(toAgent) };

		float amountOfOverlap{ allAgents[i]->mRadius + mRadius - distanceFromEachOther };

		if (amountOfOverlap >= 0.0f)
		{
			mPosition += (toAgent / distanceFromEachOther) * amountOfOverlap;
		}
	}
}