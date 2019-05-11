#ifndef INCLUDED_METAGAME_TESTSTATE_H
#define INCLUDED_METAGAME_TESTSTATE_H

#include <Engine/Inc/Engine.h>

class TestState :public WOWGE::Engine::State
{
public:
	TestState();
	~TestState() override;

	void Initialize() override;
	int Update(float deltaTime) override;
	void Terminate() override;
private:
};

#endif // !INCLUDED_METAGAME_TESTSTATE_H
