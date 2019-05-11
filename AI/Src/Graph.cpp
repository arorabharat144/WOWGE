#include "Precompiled.h"
#include "Graph.h"

using namespace WOWGE;
using namespace WOWGE::AI;

Graph::Graph() :
	mNodes{ nullptr }, 
	mColumns{ 0 },
	mRows{ 0 }
{

}

Graph::~Graph()
{

}

void Graph::Init(uint32_t columns, uint32_t rows, float cellSize)
{
	mNodes = std::make_unique<Node[]>(columns * rows);
	mColumns = columns;
	mRows = rows;

	for (uint32_t y = 0; y < mRows; ++y)
	{
		for (uint32_t x = 0; x < mColumns; ++x)
		{
			GetNode({ x, y }).coord = { x, y };
		}
	}

	mCellSize = cellSize;
}

void Graph::Clear()
{
	mNodes.reset();
	mColumns = 0;
	mRows = 0;
}

Graph::Node& Graph::GetNode(Coord coord)
{
	ASSERT(coord.x < mColumns && coord.y < mRows, "[Graph] Invalid coordinates");
	return mNodes[coord.x + (coord.y * mColumns)];
}

bool Graph::RunBFS(Coord start, Coord end)
{
	Reset();

	mOpenList.push_back(start);
	GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front();
		mOpenList.pop_front();

		if (current == end)
		{
			found = true;
		}
		else
		{
			//Loop current.x/y +-1
			//If not blocked/open/close
			//Add to open list
			std::list<Coord> neighbours{ GetNeighbours(current) };
			for (auto it = neighbours.begin(); it != neighbours.end(); ++it)
			{
				auto& neighbour{ GetNode(*it) };

				if (!neighbour.blocked && 
					!neighbour.closed &&
					!neighbour.opened)
				{
					mOpenList.push_back(*it);

					neighbour.opened = true;
					neighbour.parent = &GetNode(current);
				}
			}
		}
		mClosedList.push_back(current);
		GetNode(current).closed = true;
	}

	return found;
}

bool Graph::RunDFS(Coord start, Coord end)
{
	Reset();

	mOpenList.push_front(start);
	GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front();
		mOpenList.pop_front();

		if (current == end)
		{
			found = true;
		}
		else
		{
			//Loop current.x/y +-1
			//If not blocked/open/close
			//Add to open list
			std::list<Coord> neighbours{ GetNeighbours(current) };
			for (auto it = neighbours.begin(); it != neighbours.end(); ++it)
			{
				auto& neighbour{ GetNode(*it) };

				if (!neighbour.blocked &&
					!neighbour.closed &&
					!neighbour.opened)
				{
					mOpenList.push_front(*it);

					neighbour.opened = true;
					neighbour.parent = &GetNode(current);
				}
			}
		}
		mClosedList.push_back(current);
		GetNode(current).closed = true;
	}

	return found;
}

bool Graph::RunDijkstra(Coord start, Coord end, CostFunc costFunc)
{
	Reset();

	mOpenList.push_back(start);
	GetNode(start).opened = true;
	GetNode(start).g = 0.0f;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front();
		mOpenList.pop_front();

		if (current == end)
		{
			found = true;
		}
		else
		{
			std::list<Coord> neighbours{ GetNeighbours(current) };
			for (auto it = neighbours.begin(); it != neighbours.end(); ++it)
			{
				//check for blocked nodes
				auto& neighbour{ GetNode(*it) };
				if (!neighbour.opened && !neighbour.blocked)
				{
					neighbour.g = GetNode(current).g + costFunc(GetNode(current).coord, neighbour.coord);
					neighbour.parent = &GetNode(current);

					//Inserting the node in a sorted list based on g-cost
					auto iterator = mOpenList.begin();
					for (; iterator != mOpenList.end(); ++iterator)
					{
						if (GetNode(*iterator).g > neighbour.g)
						{
							break;
						}
					}
					mOpenList.insert(iterator, *it);
					neighbour.opened = true;
				}
				if (!neighbour.closed)
				{
					float newG = GetNode(current).g + costFunc(GetNode(current).coord, neighbour.coord);
					if (newG < neighbour.g)
					{
						neighbour.g = newG;
						neighbour.parent = &GetNode(current);

						//Sort the openlist based on g-cost
						mOpenList.sort([this](Coord coord1, Coord coord2) 
						{
							return GetNode(coord1).g < GetNode(coord2).g;
						});
					}
				}
			}
		}
		mClosedList.push_back(current);
		GetNode(current).closed = true;
	}
	return found;
}

bool Graph::RunDijkstra(Coord start, uint32_t entityType, CostFunc costFunc)
{
	//Reset();
	//
	//mOpenList.push_back(start);
	//GetNode(start).opened = true;
	//GetNode(start).g = 0.0f;
	//
	//bool found{ false };
	
	return true;
}

bool Graph::RunAStar(Coord start, Coord end, CostFunc costFunc, HeuristicFunc heuristicFunc)
{
	Reset();

	mOpenList.push_back(start);
	GetNode(start).opened = true;
	GetNode(start).g = 0.0f;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front();
		mOpenList.pop_front();

		if (current == end)
		{
			found = true;
		}
		else
		{
			std::list<Coord> neighbours{ GetNeighbours(current) };
			for (auto it = neighbours.begin(); it != neighbours.end(); ++it)
			{
				//check for blocked nodes
				auto& neighbour{ GetNode(*it) };
				if (!neighbour.opened && !neighbour.blocked)
				{
					neighbour.g = GetNode(current).g + costFunc(GetNode(current).coord, neighbour.coord);
					neighbour.h = heuristicFunc(neighbour.coord, GetNode(end).coord);
					neighbour.parent = &GetNode(current);

					//Inserting the node in a sorted list based on g-cost
					auto iterator = mOpenList.begin();
					for (; iterator != mOpenList.end(); ++iterator)
					{
						if (GetNode(*iterator).g + GetNode(*iterator).h > neighbour.g + neighbour.h)
						{
							break;
						}
					}
					mOpenList.insert(iterator, *it);
					neighbour.opened = true;
				}
				if (!neighbour.closed)
				{
					float newG = GetNode(current).g + costFunc(GetNode(current).coord, neighbour.coord);
					if (newG < neighbour.g)
					{
						neighbour.g = newG;
						neighbour.parent = &GetNode(current);

						//Sort the openlist based on g-cost + h-cost
						mOpenList.sort([this](Coord coord1, Coord coord2)
						{
							return GetNode(coord1).g + GetNode(coord1).h < GetNode(coord2).g + GetNode(coord2).h;
						});
					}
				}
			}
		}
		mClosedList.push_back(current);
		GetNode(current).closed = true;
	}
	return found;
}

std::list<Graph::Node> Graph::GetSearch()
{
	std::list<Node> searchResult;
	for (auto it = mClosedList.begin(); it != mClosedList.end(); ++it)
	{
		searchResult.push_front(GetNode(*it));
	}
	return searchResult;
}

std::vector<Graph::Coord> Graph::GetPath()
{
	std::vector<Coord> path;
	auto it = mClosedList.back();
	while (GetNode(it).parent != nullptr)
	{
		path.push_back(it);

		it = GetNode(it).parent->coord;
	}

	path.push_back(it);

	return path;
}

void Graph::Reset()
{
	for (uint32_t i = 0; i < mColumns * mRows; ++i)
	{
		auto& node = mNodes[i];
		node.parent = nullptr;
		node.g = 0.0f;
		node.h = 0.0f;
		node.opened = false;
		node.closed = false;
	}
	mOpenList.clear();
	mClosedList.clear();
}

std::list<Graph::Coord> Graph::GetNeighbours(Coord coord)
{
	std::list<Coord> neighbours;

	Coord current = coord;

	if (current.x != 0)
	{
		current.x -= 1;
		neighbours.push_back(current);
		current = coord;
	}
	if (current.y != 0)
	{
		current.y -= 1;
		neighbours.push_back(current);
		current = coord;
	}
	if (current.x != 0 && current.y != 0)
	{
		current.x -= 1;
		current.y -= 1;
		neighbours.push_back(current);
		current = coord;
	}

	if (current.x != mColumns - 1)
	{
		current.x += 1;
		neighbours.push_back(current);
		current = coord;
	}

	if (current.y != mRows - 1)
	{
		current.y += 1;
		neighbours.push_back(current);
		current = coord;
	}
	if (current.x != mColumns - 1 && current.y != mRows - 1)
	{
		current.x += 1;
		current.y += 1;
		neighbours.push_back(current);
		current = coord;
	}

	if (current.x != 0 && current.y != mRows - 1)
	{
		current.x -= 1;
		current.y += 1;
		neighbours.push_back(current);
		current = coord;
	}
	if (current.x != mColumns - 1 && current.y != 0)
	{
		current.x += 1;
		current.y -= 1;
		neighbours.push_back(current);
		current = coord;
	}
	
	return neighbours;
}