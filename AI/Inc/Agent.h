#ifndef INCLUDED_AI_AGENT_H
#define INCLUDED_AI_AGENT_H

namespace WOWGE
{
namespace AI 
{

class AIWorld;

class Agent
{
public:
	Agent(AIWorld& world);
	virtual ~Agent();

	Maths::Matrix44 LocalToWorld();

	std::vector<Agent*> CalculateNeighbourhood(const Maths::Circle& range);
	std::vector<Maths::Sphere> CalculateObstaclesInRange(const Maths::Circle& range);
	std::vector<Maths::AABB> CalculateWallsInRange(const Maths::Circle& range);

	void EnforceNonPenetrationConstraint();

	AIWorld& World() const { return mWorld; }

	Maths::Vector3& Position() { return mPosition; }
	const Maths::Vector3& Position() const { return mPosition; }

	Maths::Vector3& Velocity() { return mVelocity; }
	const Maths::Vector3& Velocity() const { return mVelocity; }

	Maths::Vector3& Destination() { return mDestination; }
	const Maths::Vector3& Destination() const { return mDestination; }

	Maths::Vector3& Heading() { return mHeading; }
	const Maths::Vector3& Heading() const { return mHeading; }

	Maths::Vector3& Side() { return mSide; }
	const Maths::Vector3& Side() const { return mSide; }

	float& MaxSpeed() { return mMaxSpeed; }
	float MaxSpeed() const { return mMaxSpeed; }

	float& Radius() { return mRadius; }
	float Radius() const { return mRadius; }

	std::vector<Agent*> Neighbours() const { return mNeighbours; }
	std::vector<Maths::Sphere> ObstaclesInRange() const { return mObstacles; }
	std::vector<Maths::AABB> WallsInRange() const { return mWalls; }

protected:
	AIWorld& mWorld;
	Maths::Vector3 mPosition;
	Maths::Vector3 mVelocity;
	Maths::Vector3 mDestination;
	Maths::Vector3 mHeading;

	std::vector<Agent*> mNeighbours;
	std::vector<Maths::Sphere> mObstacles;
	std::vector<Maths::AABB> mWalls;

	float mRadius{ 1.0f };

	Maths::Vector3 mSide;

	float mMaxSpeed;
};

using AgentList = std::vector<Agent*>;
using ObstacleList = std::vector<Maths::Sphere>;
using WallList = std::vector<Maths::AABB>;

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_AGENT_H