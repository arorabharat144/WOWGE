#include "Talk.h"

void Talk::Enter(OfficeWorker& agent)
{
	agent.SetCurrentAnimation("Talking", true);
}

void Talk::Update(OfficeWorker& agent, float deltaTime)
{

}

void Talk::Exit(OfficeWorker& agent)
{

}