#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace WOWGE;
using namespace WOWGE::AI;

PerceptionModule::PerceptionModule(Agent& agent, ImportanceCalculator calculator)
	: mAgent(agent)
	, mCalculator(calculator)
{}

PerceptionModule::~PerceptionModule()
{
	ASSERT(mSensors.empty(), "[PerceptionModule] Purge() must be called before destruction.");
}

void PerceptionModule::Purge()
{
	mSensors.clear();
}

void PerceptionModule::Update(float deltaTime)
{
	for (auto& s : mSensors)
	{
		s->Update(mAgent, mMemory, deltaTime);
	}

	for (auto& m : mMemory)
	{
		mCalculator(m.get());
	}

	mMemory.sort([](std::unique_ptr<MemoryRecord>& a, std::unique_ptr<MemoryRecord>& b)
	{
		return a->importance > b->importance;
	});

}