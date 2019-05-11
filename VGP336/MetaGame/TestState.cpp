#include "TestState.h"

#include "Metas.h"

using namespace WOWGE;

TestState::TestState()
{

}
TestState::~TestState()
{

}

void TestState::Initialize()
{
	Car::StaticGetMetaClass()->Dump();
	Ferrari::StaticGetMetaClass()->Dump();
}
int TestState::Update(float deltaTime)
{
	Graphics::SimpleDraw::AddScreenCircle({ 1920.0f * 0.5f, 1080.0f * 0.5f }, 100.0f, Maths::Vector4::Red());
	auto inputSystem = Input::InputSystem::Get();

	if (inputSystem->IsKeyPressed(Input::KeyCode::TWO))
	{
		return 1;
	}

	return -1;
}

void TestState::Terminate()
{

}