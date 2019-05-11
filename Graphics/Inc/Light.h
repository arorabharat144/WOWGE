#ifndef INCLUDED_GRAPHICS_LIGHT_HEADER
#define INCLUDED_GRAPHICS_LIGHT_HEADER

#include <Math/Inc/Maths.h>

namespace WOWGE
{
namespace Graphics
{
struct  Light
{
	Light() :
		direction(0.0f, 0.0f, 1.0f, 0.0f),
		ambient(Maths::Vector4::White()),
		diffuse(Maths::Vector4::White()),
		specular(Maths::Vector4::White()) {}

	Maths::Vector4 direction;
	Maths::Vector4 ambient;
	Maths::Vector4 diffuse;
	Maths::Vector4 specular;
};


} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_LIGHT_HEADER
