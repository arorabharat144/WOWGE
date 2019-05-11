#include "Precompiled.h"

#include "WallAvoidanceBehaviour.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace WOWGE;
using namespace WOWGE::AI;

namespace
{
	Maths::Vector3 GetNormal(const Maths::AABB& aabb, const Maths::Vector3& point)
	{
		Maths::Vector3 normal;
		Maths::Vector3 localPoint{ point - aabb.center };

		float min{ FLT_MAX };
		float distance{ Maths::Absolute(aabb.extend.x - Maths::Absolute(localPoint.x)) };
		if (distance < min)
		{
			min = distance;
			normal = { 1.0f, 0.0f, 0.0f };
			normal = { copysign(normal.x, localPoint.x), 0.0f, 0.0f };
		}
		//distance = Maths::Absolute(aabb.extend.y - Maths::Absolute(localPoint.y));
		//if (distance < min)
		//{
		//	min = distance;
		//	normal = { 0.0f, 1.0f, 0.0f };
		//	normal = { 0.0f, copysign(normal.y, localPoint.y), 0.0f };
		//}
		distance = Maths::Absolute(aabb.extend.z - Maths::Absolute(localPoint.z));
		if (distance < min)
		{
			min = distance;
			normal = { 0.0f, 0.0f, 1.0f };
			normal = { 0.0f, 0.0f, copysign(normal.z, localPoint.z) };
		}

		return normal;
	}
}

WallAvoidanceBehaviour::WallAvoidanceBehaviour()
{

}

Maths::Vector3 WallAvoidanceBehaviour::Calculate(Agent& agent)
{
	CreateFeelers(agent);

	float distanceToIntersectionPoint{ 0.0f };
	float distanceToClosestIntersectionPoint{ FLT_MAX };

	int closestWall{ -1 };

	Maths::Vector3 steeringForce;
	//Maths::Vector3 point;
	Maths::Vector3 closestPoint;

	WallList walls{ agent.World().GetWalls() };

	for (uint32_t i{ 0 }; i < 3; ++i)
	{
		Maths::Vector3 normal;
		for (uint32_t j{ 0 }; j < walls.size(); ++j)
		{
			if (Maths::Intersect(Maths::LineSegment{ agent.Position(), mFeelers[i] }, walls[j], distanceToIntersectionPoint, point))
			{
				if (distanceToIntersectionPoint < distanceToClosestIntersectionPoint)
				{
					distanceToClosestIntersectionPoint = distanceToIntersectionPoint;

					closestWall = j;

					closestPoint = point;
				}
			}
		}

		if (closestWall >= 0)
		{
			Maths::Vector3 overshoot{ mFeelers[i] - closestPoint };
			Maths::Vector3 normal{ GetNormal(walls[closestWall], closestPoint) };
			steeringForce = normal * Maths::Magnitude(overshoot);

		}
	}

	return steeringForce;
}

void WallAvoidanceBehaviour::CreateFeelers(Agent& agent)
{
	//Front feeler
	mFeelers[0] = agent.Position() + agent.Heading() * mFeelerLength;

	//Left feeler
	Maths::Vector3 temp{ agent.Heading() };
	Maths::Matrix44 rotationMatrix{ Maths::Matrix44::RotationY(Maths::ConvertDegreeToRadian(-30.0f)) };
	Maths::Matrix44 translation{ Maths::Matrix44::Translation(temp) * rotationMatrix };
	temp = Maths::GetTranslation(translation);
	mFeelers[1] = agent.Position() + temp * mFeelerLength * 0.5f;

	//Right feeler
	temp = agent.Heading();
	rotationMatrix = Maths::Matrix44::RotationY(Maths::ConvertDegreeToRadian(30.0f));
	translation = Maths::Matrix44::Translation(temp) * rotationMatrix;
	temp = Maths::GetTranslation(translation);
	mFeelers[2] = agent.Position() + temp * mFeelerLength * 0.5f;
}