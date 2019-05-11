#include "GameObjectUser.h"

#include <Engine/Inc/Engine.h>

using namespace WOWGE;

GameObjectUser::GameObjectUser() :
	mGameObjectAllocator{ 512 },
	mGameObjectFactory{ mGameObjectAllocator }
{

}

GameObjectUser::~GameObjectUser()
{

}

void GameObjectUser::Initialize()
{
	mBox[0] = mGameObjectFactory.Create("..\\..\\Assets\\Templates\\blue_box.xml");
	mBox[1] = mGameObjectFactory.Create("..\\..\\Assets\\Templates\\green_box.xml");
	mBox[2] = mGameObjectFactory.Create("..\\..\\Assets\\Templates\\red_box.xml");

	for (uint32_t i{ 0 }; i < 3; ++i)
	{
		mBox[i]->Initialize();
	}
}

int GameObjectUser::Update(float deltaTime)
{
	auto inputSystem = Input::InputSystem::Get();

	for (uint32_t i{ 0 }; i < 3; ++i)
	{
		mBox[i]->Render();
	}

	return -1;
}

void GameObjectUser::Terminate()
{
	for (uint32_t i{ 0 }; i < 3; ++i)
	{
		mBox[i]->Terminate();
	}
}