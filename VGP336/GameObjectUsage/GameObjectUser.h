#ifndef INCLUDED_GAMEOBJECTUSAGE_GAMEOBJECTUSER_H
#define INCLUDED_GAMEOBJECTUSAGE_GAMEOBJECTUSER_H

#include <Engine/Inc/Engine.h>
class GameObjectUser :public WOWGE::Engine::State
{
public:
	GameObjectUser();
	~GameObjectUser() override;

	void Initialize() override;
	int Update(float deltaTime) override;
	void Terminate() override;
private:
	WOWGE::Engine::GameOjectAllocator mGameObjectAllocator;
	WOWGE::Engine::GameObjectFactory mGameObjectFactory;

	WOWGE::Engine::GameObject* mBox[3];
};

#endif // !INCLUDED_GAMEOBJECTUSAGE_GAMEOBJECTUSER_H
