#include "Precompiled.h"
#include "Constraints.h"

#include "Particle.h"

using namespace WOWGE;
using namespace WOWGE::Physics;

FixedConstraint::FixedConstraint(Particle* particle, const Maths::Vector3& extent) :
	mParticle{ particle },
	mExtent{ extent }
{
	if ((mExtent.x < mParticle->radius) && (mExtent.y < mParticle->radius) && (mExtent.z < mParticle->radius))
	{
		mExtent = { mParticle->radius * 2.0f, mParticle->radius * 2.0f, mParticle->radius * 2.0f };
	}

	mAABB.center = mParticle->position;
	mAABB.extend = mExtent;
}

void FixedConstraint::Apply() const
{
	//mParticle->SetPosition(mAABB.center);
	mParticle->SetPosition(Maths::Vector3Min(Maths::Vector3Max(mParticle->position, mAABB.GetMin()), mAABB.GetMax()));
}
void FixedConstraint::DebugDraw() const
{
	Graphics::SimpleDraw::AddAABB(mAABB, Maths::Vector4::Red());
}

Spring::Spring(Particle* a, Particle* b, float restLength):
	mParticleA{a},
	mParticleB{b},
	mRestLength{restLength}
{
	if (restLength <= 0.0f)
	{
		mRestLength = Maths::Distance(mParticleB->position, mParticleA->position);
	}
}

void Spring::Apply() const
{
	Maths::Vector3 delta{ mParticleB->position - mParticleA->position };
	float deltaLength{ Maths::Magnitude(delta) };

	if (deltaLength > 0.0f)
	{
		float diff{ (deltaLength - mRestLength) / (deltaLength * (mParticleA->invMass + mParticleB->invMass)) };

		mParticleA->position += delta * mParticleA->invMass * diff;
		mParticleB->position -= delta * mParticleB->invMass * diff;
	}
}

void Spring::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(mParticleA->position, mParticleB->position, Maths::Vector4::Green());
}