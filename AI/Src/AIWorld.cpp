#include "Precompiled.h"
#include "AIWorld.h"

using namespace WOWGE;
using namespace WOWGE::AI;

AIWorld::AIWorld()
{

}

AIWorld::~AIWorld()
{

}

void AIWorld::RegisterAgent(Agent* agent)
{
	mAgents.push_back(agent);
}
void AIWorld::UnregisterAgent(Agent* agent)
{
	auto iter{ std::find(mAgents.begin(), mAgents.end(), agent) };
	if (iter != mAgents.end())
	{
		mAgents.erase(iter);
	}
}

void AIWorld::RegisterEntity(Entity* entity)
{
	mEntities.emplace_back(entity);
}

void AIWorld::UnregisterEntity(Entity* entity)
{
	auto iter{ std::find(mEntities.begin(), mEntities.end(), entity) };
	if (iter != mEntities.end())
	{
		mEntities.erase(iter);
	}
}

bool AIWorld::CheckIfEntityExists(uint32_t type)
{
	for (uint32_t i{ 0 }; i < mEntities.size(); ++i)
	{
		if (mEntities[i]->Type() == type)
		{
			return true;
		}
	}
	return false;
}

bool AIWorld::HasLOS(const Maths::LineSegment& linesegment)
{
	for (uint32_t i{ 0 }; i < mObstacles.size(); ++i)
	{
		if (Maths::Intersect(linesegment, mObstacles[i]))
		{
			return true;
		}
	}

	for (uint32_t i{ 0 }; i < mWalls.size(); ++i)
	{
		if (Maths::Intersect(linesegment, mWalls[i]))
		{
			return true;
		}
	}

	return false;
}

void AIWorld::AddObstacles(const Maths::Sphere& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AIWorld::AddWall(const Maths::AABB& wall)
{
	mWalls.push_back(wall);
}