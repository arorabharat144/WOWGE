#ifndef INCLUDED_PHYSICS_CONSTRAINTS_H
#define INCLUDED_PHYSICS_CONSTRAINTS_H

namespace WOWGE
{
namespace Physics
{

class Particle;

class Constraint
{
public:
	virtual ~Constraint() {}

	virtual void Apply() const = 0;
	virtual void DebugDraw() const {}
};

class FixedConstraint : public Constraint
{
public:
	FixedConstraint(Particle* particle, const Maths::Vector3& extent = {-1.0f, -1.0f, -1.0f});
	void Apply() const override;
	void DebugDraw() const override;

protected:
	Particle* mParticle;

	Maths::AABB mAABB;
	Maths::Vector3 mExtent;
};

class Spring : public Constraint
{
public:
	Spring(Particle* a, Particle* b, float restLength = -1.0f);

	void Apply() const override;
	void DebugDraw() const override;

protected:
	Particle* mParticleA;
	Particle* mParticleB;
	float mRestLength;
};

} //namespace Physics
} //namespace WOWGE

#endif // !INCLUDED_PHYSICS_CONSTRAINTS_H
