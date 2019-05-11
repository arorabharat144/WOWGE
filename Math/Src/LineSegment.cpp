#include "Precompiled.h"
#include "LineSegment.h"
#include "Sphere.h"
#include "AABB.h"

using namespace WOWGE;
using namespace WOWGE::Maths;

bool WOWGE::Maths::Intersect(const LineSegment& lineSegment, const Sphere& sphere)
{
	Vector3 startToCenter{ sphere.center - lineSegment.from };
	Vector3 startToEnd{ lineSegment.to - lineSegment.from };

	float length{ Magnitude(startToEnd) };

	Vector3 direction{ startToEnd / length };

	// Find the closest point to the line segment
	float projection{ Dot(startToCenter, direction) };
	Vector3 closestPoint;
	if (projection > length)
	{
		closestPoint = lineSegment.to;
	}
	else if (projection < 0.0f)
	{
		closestPoint = lineSegment.from;
	}
	else
	{
		closestPoint = lineSegment.from + (direction * projection);
	}

	// Check if the closest point is within the sphere
	Vector3 closestToCenter{ sphere.center - closestPoint };
	if (MagnitudeSquare(closestToCenter) > (sphere.radius * sphere.radius))
	{
		return false;
	}
	return true;
}

bool WOWGE::Maths::Intersect(const LineSegment& lineSegment, const AABB& aabb)
{
	return Intersect(lineSegment.from, lineSegment.to, aabb);
}

bool WOWGE::Maths::Intersect(const LineSegment& lineSegment, const AABB& aabb, float& distance, Vector3& contactPoint)
{
	return Intersect(lineSegment.from, lineSegment.to, aabb, distance, contactPoint);
}