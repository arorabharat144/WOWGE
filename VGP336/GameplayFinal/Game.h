#ifndef INCLUDED_GAMEPLAYFINAL_GAME_H
#define INCLUDED_GAMEPLAYFINAL_GAME_H

#include <Engine/Inc/Engine.h>
class Game :public WOWGE::Engine::State
{
public:
	Game();
	~Game() override;

	void Initialize() override;
	void Terminate() override;
	int Update(float deltaTime) override;
private:
	//WOWGE::Engine::World mWorld;
};

#endif // !INCLUDED_GAMEPLAYFINAL_GAME_H
