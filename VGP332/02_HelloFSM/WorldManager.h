#ifndef INCLUDED_WORLDMANAGER_H
#define INCLUDED_WORLDMANAGER_H

#include <Core/Inc/Core.h>
#include <Math/Inc/Maths.h>

class WorldManager
{
public:
	static void StaticInitialize();

	static std::shared_ptr<WorldManager> Get();

public:
	WorldManager();
	~WorldManager();

	void Initialize();
	void Terminate();

	void Update(float deltaTime);

	WOWGE::Maths::Vector3& MiningLocation() { return mMiningLocation; }
	const WOWGE::Maths::Vector3& MiningLocation() const { return mMiningLocation; }

	WOWGE::Maths::Vector3& EnemyLocation() { return mEnemyLocation; }
	const WOWGE::Maths::Vector3& EnemyLocation() const { return mEnemyLocation; }

	WOWGE::Maths::Vector3& HomeLocation() { return mHomeLocation; }
	const WOWGE::Maths::Vector3& HomeLocation() const { return mHomeLocation; }



	uint32_t& IronReserves() { return mIronReserves; }
	const uint32_t& IronReserves() const { return mIronReserves; }

	uint32_t& EnemyHealth() { return mEnemyHealth; }
	const uint32_t& EnemyHealth() const { return mEnemyHealth; }

	uint32_t& RestPercent() { return mRestPercent; }
	const uint32_t& RestPercent() const { return mRestPercent; }

private:

	WOWGE::Maths::Vector3 mMiningLocation;
	WOWGE::Maths::Vector3 mEnemyLocation;
	WOWGE::Maths::Vector3 mHomeLocation;

	uint32_t mIronReserves{ 0 };
	uint32_t mEnemyHealth{ 100 };
	uint32_t mRestPercent{ 0 };
};

#endif // !INCLUDED_WORLDMANAGER_H
