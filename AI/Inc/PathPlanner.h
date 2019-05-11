#ifndef INCLUDED_AI_PATHPLANNER_H
#define INCLUDED_AI_PATHPLANNER_H

#include "Graph.h"

namespace WOWGE
{
namespace AI
{

class Agent;

class PathPlanner
{
public:
	PathPlanner(Agent& agent);
	void RequestPath(const Maths::Vector3& destination);
	void RequestPath(uint32_t type);

	const Path& GetPath() const { return mPath; }

	void SetCostFunc(Graph::CostFunc costFunc) { mCostFunc = costFunc; }
	void SetHeuristicFunc(Graph::HeuristicFunc heuristicFunc) { mHeuristicFunc = heuristicFunc; }

private:
	//Graph::Coord GetClosestNodeCoord(const Maths::Vector3& position);

private:
	Agent& mAgent;
	Path mPath;

	Graph::CostFunc mCostFunc;
	Graph::HeuristicFunc mHeuristicFunc;
};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_PATHPLANNER_H