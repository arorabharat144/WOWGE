#ifndef INCLUDED_ENGINE_GAMEAPP_H
#define INCLUDED_ENGINE_GAMEAPP_H

#include "StateMachine.h"

#include "World.h"

namespace WOWGE
{
namespace Engine
{

class GameApp : public Core::Application
{
public:
	GameApp();
	~GameApp() override;

	StateMachine<GameApp>& GetStateMachine() { return mStateMachine; }

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate() override;

	Core::Window mWindow;
	Core::Timer mTimer;

	World mWorld;

	StateMachine<GameApp> mStateMachine;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_GAMEAPP_H
