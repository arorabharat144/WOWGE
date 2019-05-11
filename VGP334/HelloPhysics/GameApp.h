#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <Input/Inc/Input.h>
#include <Physics/Inc/Physics.h>

#include <External/ImGui/Inc/imgui.h>

class GameApp : public WOWGE::Core::Application
{
public:
	GameApp();
	~GameApp() override;

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate() override;

	WOWGE::Core::Window mWindow;
	WOWGE::Core::Timer mTimer;

	WOWGE::Graphics::Camera mCamera;

	WOWGE::Maths::Plane* mPlane;
	WOWGE::Physics::PhysicsWorld mPhysicsWorld;
};

#endif // !INCLUDED_GAMEAPP_H
