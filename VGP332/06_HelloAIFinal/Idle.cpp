#include "Idle.h"

void Idle::Enter(OfficeWorker& agent)
{
	agent.MaxSpeed() = 0.0f;
	agent.SetCurrentAnimation("Idle", true);
}

void Idle::Update(OfficeWorker& agent, float deltaTime)
{

}

void Idle::Exit(OfficeWorker& agent)
{

}