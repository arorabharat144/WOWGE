#ifndef INCLUDED_METAGAME_METAS_H
#define INCLUDED_METAGAME_METAS_H

#include <Engine/Inc/Engine.h>

class Car
{
public:
	META_CLASS_DECLARE

	int position;
	float fuel;
private:
};

class Ferrari : public Car
{
public:
	META_CLASS_DECLARE

	bool turbo;
};

#endif // !INCLUDED_METAGAME_METAS_H
