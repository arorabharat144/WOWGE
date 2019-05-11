#ifndef INCLUDED_MATH_AABB_H
#define INCLUDED_MATH_AABB_H

#include "Vector3.h"

namespace WOWGE
{
namespace Maths
{

struct Sphere;

struct AABB
{
	AABB() {}
	AABB(const Maths::Vector3& centerParam, const Maths::Vector3& extendParam) :
		center(centerParam),
		extend(extendParam) {}

	Maths::Vector3 GetMin() const
	{
		Maths::Vector3 temp;
		temp = center - extend;
		return temp;
	}
	Maths::Vector3 GetMax() const
	{
		Maths::Vector3 temp;
		temp = center + extend;
		return temp;
	}

	void SetMinMax(const Maths::Vector3& min, const Maths::Vector3& max)
	{
		center = (min + max) * 0.5f;
		extend = max - center;
	}

	Maths::Vector3 center;
	Maths::Vector3 extend;
};

bool Intersect(float rectMin, float rectMax, float lineStart, float lineEnd, float& enter, float& exit);
bool Intersect(const Vector3& point1, const Vector3& point2, const AABB& aabb);
bool Intersect(const Vector3& point1, const Vector3& point2, const AABB& aabb, float& distance, Vector3& contactPoint);


bool Intersect(const Vector3& point, const AABB& aabb);

bool Intersect(const Sphere& sphere, const AABB& aabb);

} //Namespace Maths
} //Namespace WOWGE

#endif // !INCLUDED_MATH_AABB_H
