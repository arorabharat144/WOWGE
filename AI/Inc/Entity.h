#ifndef INCLUDED_AI_ENTITY_H
#define INCLUDED_AI_ENTITY_H

namespace WOWGE
{
namespace AI
{

class Entity
{
public:

	const Maths::Vector3 Position() const { return mPosition; }
	Maths::Vector3& Position() { return mPosition; }

	const uint32_t Type() const { return mType; }
	uint32_t& Type() { return mType; }

	const uint32_t ID() const { return mID; }
	uint32_t& ID() { return mID; }

protected:
	//Maths::Matrix44 mWorldTransform;

	Maths::Vector3 mPosition;
	uint32_t mType;
	uint32_t mID;
};

using EntityList = std::vector<Entity*>;

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_ENTITY_H