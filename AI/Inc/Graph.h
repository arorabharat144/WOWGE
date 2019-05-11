#ifndef INCLUDED_AI_GRAPH_H
#define INCLUDED_AI_GRAPH_H

namespace WOWGE
{
namespace AI
{

class Graph
{
public:
	struct Coord
	{
		Coord() {}

		Coord(uint32_t xParam, uint32_t yParam) : x{ xParam }, y{ yParam } {}

		bool operator==(const Coord& other) const
		{
			return x == other.x && y == other.y;
		}

		uint32_t x{ 0 }, y{ 0 };
	};
	struct Node
	{
		Coord coord;
		bool blocked{ false };

		Node* parent{ nullptr };
		float g{ 0.0f }, h{ 0.0f }; //g-cost(for dijkstra and a*), heuristic cost(for a*)
		bool opened{ false };
		bool closed{ false };
	};

	using CostFunc = std::function<float(Coord, Coord)>;
	using HeuristicFunc = std::function<float(Coord, Coord)>;

	Graph();
	~Graph();

	void Init(uint32_t columns, uint32_t rows, float cellSize);
	void Clear();

	Node& GetNode(Coord coord);

	bool RunBFS(Coord start, Coord end);
	bool RunDFS(Coord start, Coord end);
	bool RunDijkstra(Coord start, Coord end, CostFunc costFunc);
	bool RunDijkstra(Coord start, uint32_t entityType, CostFunc costFunc);
	bool RunAStar(Coord start, Coord end, CostFunc costFunc, HeuristicFunc heuristicFunc);

	std::list<Node> GetSearch();
	std::vector<Coord> GetPath();

	uint32_t GetColumns() const { return mColumns; }
	uint32_t GetRows() const { return mRows; }

	float GetCellSize() const { return mCellSize; }
	void SetCellSize(float cellSize) { mCellSize = cellSize; }

private:
	void Reset();

	std::list<Coord> GetNeighbours(Coord coord);

	//Graph State
	std::unique_ptr<Node[]> mNodes;
	uint32_t mColumns;
	uint32_t mRows;

	//Search State
	std::list<Coord> mOpenList;
	std::list<Coord> mClosedList;

	//Cell Size
	float mCellSize{ 0.0f };
};

using Path = std::vector<Graph::Coord>;

} //namespace AI
} //namespace WOWGE

#endif // !INCLUDED_AI_GRAPH_H
