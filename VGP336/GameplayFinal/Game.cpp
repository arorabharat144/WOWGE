#include "Game.h"

#include <Engine/Inc/Engine.h>

using namespace WOWGE;

Game::Game()
{

}

Game::~Game()
{

}

void Game::Initialize()
{
	//mWorld.Initialize(100);

	//mWorld.LoadLevel("../../Assets/Templates/Level_01.xml");
}

void Game::Terminate()
{
	
}

int Game::Update(float deltaTime)
{
	auto inputSystem = Input::InputSystem::Get();

	//mWorld.Update(deltaTime);

	//mWorld.Render();

	return -1;
}