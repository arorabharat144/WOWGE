#ifndef INCLUDED_DESTINATION_H
#define INCLUDED_DESTINATION_H

#include <AI/Inc/Graph.h>
#include <Math/Inc/Maths.h>

struct DestinationType 
{
	std::string name;
	WOWGE::AI::Graph::Coord coordinate;

	WOWGE::Maths::Vector3 normal;

	bool spotTaken{ false };
};

#endif // !INCLUDED_OFFICEWORKER_H