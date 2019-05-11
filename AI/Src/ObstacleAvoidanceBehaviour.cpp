#include "Precompiled.h"

#include "ObstacleAvoidanceBehaviour.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace WOWGE;
using namespace WOWGE::AI;

ObstacleAvoidanceBehaviour::ObstacleAvoidanceBehaviour()
{

}

Maths::Vector3 ObstacleAvoidanceBehaviour::Calculate(Agent& agent)
{
	//ahead = agent.Position() + Maths::Normalize(agent.Velocity()) * agent.MaxSpeed() ;
	//
	//Maths::AABB mostThreateningObstacle;
	//
	//for (uint32_t i{ 0 }; i < agent.World().GetObstacles().size(); ++i)
	//{
	//	Maths::AABB obstacle{ agent.World().GetObstacles()[i] };
	//	bool collision{ Maths::Intersect(ahead, agent.Position(), obstacle) };
	//	if (collision)
	//	{
	//		mostThreateningObstacle = obstacle;
	//	}
	//}

	//Maths::Vector3 avoidance;
	//if (mostThreateningObstacle.center != Maths::Vector3(0.0f, 0.0f, 0.0f) ||
	//	mostThreateningObstacle.extend != Maths::Vector3(0.0f, 0.0f, 0.0f))
	//{
	//	avoidance.x = ahead.x - (Maths::Absolute(mostThreateningObstacle.center.x) + Maths::Absolute(mostThreateningObstacle.extend.x));
	//	avoidance.z = ahead.z - (Maths::Absolute(mostThreateningObstacle.center.z) + Maths::Absolute(mostThreateningObstacle.extend.z));
	//
	//	avoidance = Maths::Normalize(avoidance) * mMaxAvoidForce;
	//}
	
	//return avoidance;

	mDetectionBoxLength = agent.MaxSpeed();
	ObstacleList obstacles{ agent.ObstaclesInRange() };

	for (uint32_t i{ 0 }; i < obstacles.size(); ++i)
	{
		Maths::Vector3 localPosition{ Maths::GetTranslation(Maths::Inverse(agent.LocalToWorld()) * Maths::Matrix44::Translation(obstacles[i].center)) };

		if (localPosition.x >= 0.0f)
		{
			float expandedRadius{ obstacles[i].radius + agent.Radius() };

			if (Maths::Absolute(localPosition.z) < expandedRadius)
			{
				
			}
		}
	}
	

	return Maths::Vector3();
}