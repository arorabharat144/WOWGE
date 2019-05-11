#ifndef INCLUDED_PHYSICS_PARTICLE_H
#define INCLUDED_PHYSICS_PARTICLE_H

namespace WOWGE
{
namespace Physics
{

class Particle
{
public:

	Particle() = default;

	void DebugDraw() const;

	void SetPosition(const Maths::Vector3& positionParam);
	void SetVelocity(const Maths::Vector3& velocityParam);

	Maths::Vector3 position;
	Maths::Vector3 positionOld;
	Maths::Vector3 acceleration;
	float radius{ 1.0f };
	float invMass{ 1.0f };
};

} //namespace Physics
} //namespace WOWGE

#endif // !INCLUDED_PHYSICS_PARTICLE_H
