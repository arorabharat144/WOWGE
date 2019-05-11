#ifndef INCLUDED_AI_AIWORLD_H
#define INCLUDED_AI_AIWORLD_H

#include "Agent.h"
#include "Entity.h"
#include "Graph.h"

namespace WOWGE
{
namespace AI
{

class AIWorld
{
public:
	using Obstacles = std::vector<Maths::Sphere>;
	using Walls = std::vector<Maths::AABB>;

	AIWorld();
	~AIWorld();

	void RegisterAgent(Agent* agent);
	void UnregisterAgent(Agent* agent);

	void RegisterEntity(Entity* entity);
	void UnregisterEntity(Entity* entity);

	bool CheckIfEntityExists(uint32_t type);

	bool HasLOS(const Maths::LineSegment& linesegment);

	AgentList GetAllAgents() const { return mAgents; }
	EntityList GetAllEntities() const { return mEntities; }
	
	void AddObstacles(const Maths::Sphere& obstacle);
	const Obstacles& GetObstacles() const { return mObstacles; }

	void AddWall(const Maths::AABB& wall);
	const Walls& GetWalls() const { return mWalls; }

	Graph& GetNavGraph() { return mNavGraph; }
	const Graph& GetNavGraph() const { return mNavGraph; }

private:
	AgentList mAgents;
	EntityList mEntities;

	Obstacles mObstacles;
	Walls mWalls;
	Graph mNavGraph;
	bool mInitialized;
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_AIWORLD_H