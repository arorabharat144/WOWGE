#ifndef INCLUDED_SHIP_H
#define INCLUDED_SHIP_H

#include <Math/Inc/Maths.h>
#include <AI/Inc/SteeringModule.h>
#include <AI/Inc/Agent.h>

class Ship : public WOWGE::AI::Agent
{
public:
	Ship(WOWGE::AI::AIWorld& aiWorld);

	~Ship() override;

	void Initialize(const WOWGE::Maths::Vector3& position, float maxSpeed, float mass);
	void Update(float deltaTime);

	WOWGE::AI::SteeringModule& GetSteeringModule();

	float& GetMass() { return mMass; }

private:
	WOWGE::AI::SteeringModule mSteeringModule;

	float mMass;
};

#endif // !INCLUDED_SHIP_H
