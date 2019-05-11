#ifndef INCLUDED_MATH_PLANE_H
#define INCLUDED_MATH_PLANE_H

#include "Vector3.h"

namespace WOWGE
{
namespace Maths
{

struct Plane
{
	Vector3 n;
	float d;

	Plane() :n{ 0.0f, 1.0f, 0.0f },
		d{ 0.0f } {}

	Plane(float a, float b, float c, float d) :
		n{ a, b, c }, d{ d } {}
};

//bool IsInFront(const Maths::Vector3& point, const Maths::Plane& plane);

} //namespace Maths
} //namespace WOWGE

#endif // !INCLUDED_MATH_PLANE_H
