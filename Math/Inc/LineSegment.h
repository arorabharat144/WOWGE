#ifndef INCLUDED_MATH_LINESEGMENT_H
#define INCLUDED_MATH_LINESEGMENT_H

#include "Vector3.h"

namespace WOWGE
{
namespace Maths
{

struct Sphere;
struct AABB;

struct LineSegment
{
	LineSegment() {}
	LineSegment(const Vector3& v1, const Vector3& v2) :
		from{ v1 },
		to{ v2 } {}

	Maths::Vector3 from;
	Maths::Vector3 to;
};

bool Intersect(const LineSegment& lineSegment, const Sphere& sphere);
bool Intersect(const LineSegment& lineSegment, const AABB& aabb);
bool Intersect(const LineSegment& lineSegment, const AABB& aabb, float& distance, Vector3& contactPoint);

} //Namespace Maths
} //Namespace WOWGE

#endif // !INCLUDED_MATH_SEGMENT_H
