#ifndef INCLUDED_AI_SENSOR_H
#define INCLUDED_AI_SENSOR_H

#include "MemoryRecord.h"

namespace WOWGE
{
namespace AI
{

class Agent;

class Sensor
{
public:
	Sensor() {}
	virtual ~Sensor() {}

	virtual void Update(Agent& agent, MemoryRecords& memory, float deltaTime) = 0;
	virtual const char* GetName() const = 0;

	void SetActive(bool active) { mActive = active; }
	bool IsActive() const { return mActive; }

private:
	bool mActive;
};

using Sensors = std::vector<std::unique_ptr<Sensor>>;

} // namespace AI
}

#endif // #include INCLUDED_AI_SENSOR_H