#ifndef INCLUDED_GRAPHICS_KEYFRAME_H
#define INCLUDED_GRAPHICS_KEYFRAME_H

#include <Math/Inc/Maths.h>

namespace WOWGE
{
namespace Graphics
{

template<class T>
struct Keyframe
{
	T key;
	float time{ -1.0f };
};

} //namespace Graphics
} //namespace WOWGE;

#endif // !INCLUDED_GRAPHICS_KEYFRAME_H
