#ifndef INCLUDED_PHYSICS_PHYSICSWORLD
#define INCLUDED_PHYSICS_PHYSICSWORLD

#include "Constraints.h"
#include "Particle.h"

namespace WOWGE
{
namespace Physics
{

class PhysicsWorld
{
public:
	struct Settings
	{
		Maths::Vector3 gravity{ 0.0f, -9.8f, 0.0f };
		float timeStep{ 1.0f / 60.0f };
		float drag{ 0.0f };
		uint32_t iterations{ 1 };
	};

	PhysicsWorld();
	~PhysicsWorld();

	void Setup(const Settings& settings);

	void AddParticle(Particle* particle);
	void AddConstraints(Constraint* constraint);
	void AddPlane(Maths::Plane* plane);

	void Update(float deltaTime);
	void ClearAll();

	void DebugDraw() const;

private:
	void AccumulateForces();
	void Integrate();
	void SatisfyConstraints();

private:
	std::vector<Particle*> mParticles;
	std::vector<Constraint*> mConstraints;
	std::vector<Maths::Plane*> mPlanes;

	Settings mSettings;
	float mTimer;
};

} //namespace Physics
} //namespace WOWGE

#endif // !INCLUDED_PHYSICS_PHYSICSWORLD
