#include "Precompiled.h"
#include "PathPlanner.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace WOWGE;
using namespace WOWGE::AI;

PathPlanner::PathPlanner(Agent& agent) :
	mAgent{ agent }
{

}

void PathPlanner::RequestPath(const Maths::Vector3& destination)
{
	mPath.clear();

	Graph::Coord startCoord{ static_cast<uint32_t>(mAgent.Position().x), static_cast<uint32_t>(mAgent.Position().z) };
	Graph::Coord endCoord{ static_cast<uint32_t>(destination.x), static_cast<uint32_t>(destination.z) };

	//if (mAgent.World().HasLOS({ mAgent.Position(), destination }))
	//{
	//	//mPath.emplace_back(startCoord);
	//	mPath.emplace_back(endCoord);
	//	return;
	//}

	uint32_t startGridCoordX{ static_cast<uint32_t>(0.5f * mAgent.Position().x / mAgent.World().GetNavGraph().GetCellSize()) };
	uint32_t startGridCoordZ{ static_cast<uint32_t>(0.5f * mAgent.Position().z / mAgent.World().GetNavGraph().GetCellSize()) };

	uint32_t endGridCoordX{ static_cast<uint32_t>(0.5f * destination.x / mAgent.World().GetNavGraph().GetCellSize()) };
	uint32_t endGridCoordZ{ static_cast<uint32_t>(0.5f * destination.z / mAgent.World().GetNavGraph().GetCellSize()) };

	if (mAgent.World().GetNavGraph().RunAStar({ startGridCoordX, startGridCoordZ }, { endGridCoordX, endGridCoordZ }, mCostFunc, mHeuristicFunc))
	{
		mPath = mAgent.World().GetNavGraph().GetPath();
		for (uint32_t i{ 0 }; i < mPath.size(); ++i)
		{
			mPath[i].x = static_cast<uint32_t>(2.0f * mPath[i].x * mAgent.World().GetNavGraph().GetCellSize());
			mPath[i].y = static_cast<uint32_t>(2.0f * mPath[i].y * mAgent.World().GetNavGraph().GetCellSize());
		}
	}
	else
	{
		return;
	}
	//Do path smoothing
}

void PathPlanner::RequestPath(uint32_t type)
{
	mPath.clear();

	if (mAgent.World().CheckIfEntityExists(type) == false)
	{
		return;
	}

}

//Graph::Coord PathPlanner::GetClosestNodeCoord(const Maths::Vector3& position)
//{
//
//}