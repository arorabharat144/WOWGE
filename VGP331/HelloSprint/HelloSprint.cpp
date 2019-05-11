#include "HelloSprint.h"

#include <Engine/Inc/Engine.h>

using namespace WOWGE;

HelloSprint::HelloSprint()
{

}

HelloSprint::~HelloSprint()
{

}

void HelloSprint::Initialize()
{
	mRunners.resize(2);
	for (auto& runner : mRunners)
	{
		runner.frameId = 0;
		runner.position = 100.0f;
	}
}

int HelloSprint::Update(float deltaTime)
{
	auto inputSystem = Input::InputSystem::Get();
	if (mRunners[0].frameId == 0)
	{
		if (inputSystem->IsKeyPressed(Input::KeyCode::RIGHT))
		{
			mRunners[0].frameId = 1;
			mRunners[0].position += 2.0f;
		}
	}
	else
	{
		if (inputSystem->IsKeyPressed(Input::KeyCode::LEFT))
		{
			mRunners[0].frameId = 0;
			mRunners[0].position += 2.0f;
		}
	}

	if (mRunners[1].frameId == 0)
	{
		if (inputSystem->IsKeyPressed(Input::KeyCode::D))
		{
			mRunners[1].frameId = 1;
			mRunners[1].position += 2.0f;
		}
	}
	else
	{
		if (inputSystem->IsKeyPressed(Input::KeyCode::A))
		{
			mRunners[1].frameId = 0;
			mRunners[1].position += 2.0f;
		}
	}

	std::vector<float> scales;
	if (mRunners[0].frameId == 0)
	{
		scales.emplace_back(50.0f);
	}
	else
	{
		scales.emplace_back(100.0f);
	}

	if (mRunners[1].frameId == 0)
	{
		scales.emplace_back(50.0f);
	}
	else
	{
		scales.emplace_back(100.0f);
	}

	for (uint32_t i{ 0 }; i < mRunners.size(); ++i)
	{
		Graphics::SimpleDraw::AddScreenCircle({ mRunners[i].position, 200.0f + (i * 200.0f) }, scales[i], Maths::Vector4::Red());
	}

	return -1;
}

void HelloSprint::Terminate()
{

}

void HelloSprint::Host()
{

}

void HelloSprint::Join()
{

}