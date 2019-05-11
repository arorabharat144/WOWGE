#ifndef INCLUDED_AI_PERCEPTIONMODULE_H
#define INCLUDED_AI_PERCEPTIONMODULE_H

#include "MemoryRecord.h"
#include "Sensor.h"

namespace WOWGE
{ 
namespace AI 
{

class Agent;

class PerceptionModule
{
public:
	using ImportanceCalculator = std::function<void(MemoryRecord*)>;

	PerceptionModule(Agent& agent, ImportanceCalculator calculator);
	~PerceptionModule();

	template<class SensorType>
	SensorType* AddSensor();

	template<class SensorType>
	SensorType* GetSensor(const char* name);

	void Purge();

	void Update(float deltaTime);

	MemoryRecords& GetMemoryRecords() { return mMemory; }

private:
	Agent& mAgent;
	ImportanceCalculator mCalculator;
	Sensors mSensors;
	MemoryRecords mMemory;
};

template<class SensorType>
SensorType* PerceptionModule::AddSensor()
{
	mSensors.emplace_back(std::make_unique<SensorType>());
	return static_cast<SensorType*>(mSensors.back().get());
}

template<class SensorType>
SensorType* PerceptionModule::GetSensor(const char* name)
{
	for (auto& s : mSensors)
	{
		if (strcmp(s->GetName(), name) == 0)
		{
			return static_cast<SensorType*>(s.get());
		}
	}

	return nullptr;
}

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_PERCEPTIONMODULE_H