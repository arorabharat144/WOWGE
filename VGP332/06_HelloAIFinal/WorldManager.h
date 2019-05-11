#ifndef INCLUDED_WORLDMANAGER_H
#define INCLUDED_WORLDMANAGER_H

#include <Core/Inc/Core.h>
#include <Math/Inc/Maths.h>

#include "Destination.h"

class OfficeWorker;

class WorldManager
{
public:
	static void StaticInitialize();

	static std::shared_ptr<WorldManager> Get();

public:
	WorldManager();
	~WorldManager();

	void Initialize();

	void Update(float deltaTime);

	const std::vector<DestinationType> Destinations() const { return mDestinations; }
	std::vector<DestinationType>& Destinations() { return mDestinations; }

	const bool HasStartedSimulation() const { return mHasStartedSimulation; }
	bool& HasStartedSimulation() { return mHasStartedSimulation; }

	OfficeWorker*& Boss() { return mBoss; }

	const uint32_t BossIndex() const { return mBossIndex; }
	uint32_t& BossIndex() { return mBossIndex; }

	const float MaxAgentSpeed() const { return mMaxAgentSpeed; }
	float& MaxAgentSpeed() { return mMaxAgentSpeed; }

	const float CellSize() const { return mCellSize; }
	float& CellSize() { return mCellSize; }

	const bool HasTriggeredAlarm() const { return mHasTriggeredFireAlarm; }
	bool& HasTriggeredAlarm() { return mHasTriggeredFireAlarm; }

private:
	std::vector<DestinationType> mDestinations;

	bool mHasStartedSimulation{ false };

	OfficeWorker* mBoss;
	uint32_t mBossIndex{ 0 };

	float mMaxAgentSpeed{ 50.0f };

	float mCellSize{ 20.0f };

	bool mHasTriggeredFireAlarm{ false };
};

#endif // !INCLUDED_WORLDMANAGER_H
