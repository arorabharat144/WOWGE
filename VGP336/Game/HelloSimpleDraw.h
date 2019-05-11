#ifndef INCLUDED_GAME_HELLOSIMPLEDRAW_H
#define INCLUDED_GAME_HELLOSIMPLEDRAW_H

#include <Engine/Inc/Engine.h>
class HelloSimpleDraw :public WOWGE::Engine::State
{
public:
	HelloSimpleDraw();
	~HelloSimpleDraw() override;

	void Initialize() override;
	int Update(float deltaTime) override;
	void Terminate() override;
private:

};

#endif // !INCLUDED_GAME_HELLOSIMPLEDRAW_H
