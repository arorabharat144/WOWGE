#include "Precompiled.h"
#include "PhysicsWorld.h"

using namespace WOWGE;
using namespace WOWGE::Physics;

PhysicsWorld::PhysicsWorld() :
	mTimer{ 0.0f }
{

}

PhysicsWorld::~PhysicsWorld()
{
	ClearAll();
}

void PhysicsWorld::Setup(const Settings& settings)
{
	mSettings = settings;
}

void PhysicsWorld::AddParticle(Particle* particle)
{
	mParticles.emplace_back(particle);
}

void PhysicsWorld::AddConstraints(Constraint* constraint)
{
	mConstraints.emplace_back(constraint);
}

void PhysicsWorld::AddPlane(Maths::Plane* plane)
{
	mPlanes.emplace_back(plane);
}

void PhysicsWorld::Update(float deltaTime)
{
	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;
		AccumulateForces();
		Integrate();
		SatisfyConstraints();
	}
}
void PhysicsWorld::ClearAll()
{
	for (auto particle : mParticles)
	{
		SafeDelete(particle);
	}

	for (auto constraint : mConstraints)
	{
		SafeDelete(constraint);
	}

	for (auto plane : mPlanes)
	{
		SafeDelete(plane);
	}

	mParticles.clear();
	mConstraints.clear();
	mPlanes.clear();
}

void PhysicsWorld::DebugDraw() const
{
	for (const auto particle : mParticles)
	{
		particle->DebugDraw();
	}

	for (const auto constraint : mConstraints)
	{
		constraint->DebugDraw();
	}
}

void PhysicsWorld::AccumulateForces()
{
	for (auto particle : mParticles)
	{
		particle->acceleration = mSettings.gravity;
	}
}

void PhysicsWorld::Integrate()
{
	const float timeStepSquared{ mSettings.timeStep * mSettings.timeStep };
	for (auto particle : mParticles)
	{
		Maths::Vector3 displacement{ (particle->position - particle->positionOld) + (particle->acceleration * timeStepSquared) };
		particle->positionOld = particle->position;
		particle->position = particle->position + displacement;
	}
}

void PhysicsWorld::SatisfyConstraints()
{
	//for (uint32_t i{ 0 }; i < mSettings.iterations; ++i)
	//{
	for (auto constraint : mConstraints)
	{
		constraint->Apply();
	}
	//}
	for (auto particle : mParticles)
	{
		for (auto plane : mPlanes)
		{
			Maths::Ray ray;
			ray.origin = particle->positionOld;
			ray.direction = particle->position - particle->positionOld;

			float distance{ 0.0f };
			Maths::Intersect(ray, *plane, distance);

			if (Maths::Dot(particle->position, plane->n) < 0.0f)
			{
				Maths::Vector3 contactPoint{ ray.origin + ray.direction * distance };

				Maths::Vector3 penetrationVector{ particle->position - contactPoint };
				float penetrationLength{ Maths::Magnitude(penetrationVector) };

				Maths::Vector3 reflectedVector{ Maths::Reflect(ray.direction, plane->n) };

				Maths::Vector3 velocity{ reflectedVector * penetrationLength };

				particle->SetPosition(particle->positionOld);
				particle->SetVelocity(velocity);
			}
		}
	}
}