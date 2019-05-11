#ifndef INCLUDED_MATH_SPHERE_H
#define INCLUDED_MATH_SPHERE_H

#include "Vector2.h"

namespace WOWGE
{
namespace Maths
{
struct Sphere
{
	Sphere(const Maths::Vector3& centerParam, float radiusParam) :
		center(centerParam),
		radius(radiusParam) {}

	Maths::Vector3 center;
	float radius;
};

} //Namespace Maths
} //Namespace WOWGE

#endif // !INCLUDED_MATH_SPHERE_H
