#ifndef INCLUDED_GRAPHICS_MATERIAL_HEADER
#define INCLUDED_GRAPHICS_MATERIAL_HEADER

#include <Math/Inc/Maths.h>

namespace WOWGE
{
namespace Graphics
{
struct Material
{
	Material() :
		ambient(Maths::Vector4::White()),
		diffuse(Maths::Vector4::White()),
		specular(Maths::Vector4::White()),
		emissive(Maths::Vector4::White()),
		power(1.0f){}

	Material& operator=(const Material& mat)
	{
		ambient = mat.ambient;
		diffuse = mat.diffuse;
		specular = mat.specular;
		emissive = mat.emissive;
		power = mat.power;

		return *this;
	}

	Maths::Vector4 ambient;
	Maths::Vector4 diffuse;
	Maths::Vector4 specular;
	Maths::Vector4 emissive;
	float power;
};


} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_MATERIAL_HEADER
