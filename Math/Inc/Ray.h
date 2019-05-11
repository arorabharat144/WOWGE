#ifndef INCLUDED_MATH_RAY_H
#define INCLUDED_MATH_RAY_H

#include "Vector3.h"

namespace WOWGE
{
namespace Maths
{

struct Plane;
struct AABB;

struct Ray
{
	Ray() {}
	Ray(const Vector3& originParam, const Vector3& directionParam) :
		origin{ originParam },
		direction{ directionParam } {}

	Vector3 origin;
	Vector3 direction;
};

bool Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, Vector3& contactPoint);

bool Intersect(const Ray& ray, const Plane& plane, float& distance);
bool Intersect(const Ray& ray, const Plane& plane, Vector3& contactPoint);
bool Intersect(const Ray& ray, const AABB& aabb, float& distEntry, Vector3& contactPoint);

} //namespace Maths
} //namespace WOWGE

#endif // !INCLUDED_MATH_RAY_H
