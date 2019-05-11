#ifndef INCLUDED_GAME_HELLOSPRINT_H
#define INCLUDED_GAME_HELLOSPRINT_H

#include <Engine/Inc/Engine.h>

struct Runner
{
	uint32_t frameId;
	float position;
};

class HelloSprint :public WOWGE::Engine::State
{
public:
	HelloSprint();
	~HelloSprint() override;

	void Initialize() override;
	int Update(float deltaTime) override;
	void Terminate() override;

	static void Host();
	static void Join();
private:

	std::vector<Runner> mRunners;
};

#endif // !INCLUDED_GAME_HELLOSPRINT_H
