#ifndef INCLUDED_GRAPHICS_BONE_H
#define INCLUDED_GRAPHICS_BONE_H

namespace WOWGE
{
namespace Graphics
{

struct Bone
{
	std::string name;

	Bone* parent;
	std::vector<Bone*> children;

	Maths::Matrix44 transform;
	Maths::Matrix44 offsetTransform;

	int index;
	int parentIndex;
	std::vector<int> childrenIndex;
};

} //namespace Graphics
} //namespace WOWGE
#endif // !INCLUDED_GRAPHICS_BONE_H
