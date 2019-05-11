#ifndef INCLUDED_ENTITY_H
#define INCLUDED_ENTITY_H

#include <Math/Inc/Maths.h>
#include <AI/Inc/SteeringModule.h>
#include <AI/Inc/Agent.h>

class Cylinder : public WOWGE::AI::Agent
{
public:
	Cylinder(WOWGE::AI::AIWorld& aiWorld);

	~Cylinder() override;

	void Initialize(const WOWGE::Maths::Vector3& position, float maxSpeed, float mass);
	void Update(float deltaTime);

	WOWGE::AI::SteeringModule& GetSteeringModule();

	float& GetMass() { return mMass; }

private:
	WOWGE::AI::SteeringModule mSteeringModule;
	float mMass;

	float mTimer{ 0.0f };
};

#endif // !INCLUDED_ENTITY_H
