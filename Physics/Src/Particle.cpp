#include "Precompiled.h"
#include "Particle.h"

using namespace WOWGE;
using namespace WOWGE::Physics;

void Particle::DebugDraw() const
{
	Graphics::SimpleDraw::AddSphere(position, radius, Maths::Vector4::Cyan(), 4, 2);
}

void Particle::SetPosition(const Maths::Vector3& positionParam)
{
	// positionOld = position = no motion
	position = positionParam;
	positionOld = positionParam;
}

void Particle::SetVelocity(const Maths::Vector3& velocityParam)
{
	positionOld = position - velocityParam;
}