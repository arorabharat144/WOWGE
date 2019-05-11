#ifndef INCLUDED_MATH_CIRCLE_H
#define INCLUDED_MATH_CIRCLE_H

#include "Vector2.h"

namespace WOWGE
{
namespace Maths
{
struct Circle
{
	Circle(const Maths::Vector2& centerParam, float radiusParam) :
		center(centerParam),
		radius(radiusParam) {}

	Maths::Vector2 center;
	float radius;
};



} //Namespace Maths
} //Namespace WOWGE

#endif // !INCLUDED_MATH_CIRCLE_H
