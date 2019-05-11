#include "Precompiled.h"
#include "Ray.h"
#include "Plane.h"
#include "AABB.h"

using namespace WOWGE;
using namespace WOWGE::Maths;

bool WOWGE::Maths::Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, Vector3& contactPoint)
{
	// Reference: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

	// Find vectors for two edges sharing V1
	Vector3 e1 = b - a;
	Vector3 e2 = c - a;

	// Begin calculating determinant - also used to calculate u parameter
	Vector3 P = Cross(ray.direction, e2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = Dot(e1, P);
	// NOT CULLING
	if (det <= 0.0f)
	{
		return false;
	}

	float inv_det = 1.0f / det;

	// Calculate distance from V1 to ray origin
	Vector3 T = ray.origin - a;

	// Calculate u parameter and test bound
	float u = Dot(T, P) * inv_det;

	// The intersection lies outside of the triangle
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	// Prepare to test v parameter
	Vector3 Q = Cross(T, e1);

	// Calculate V parameter and test bound
	float v = Dot(ray.direction, Q) * inv_det;

	// The intersection lies outside of the triangle
	if (v < 0.0f || u + v  > 1.0f)
	{
		return false;
	}

	// Ray intersection
	float t = Dot(e2, Q) * inv_det;
	if (t <= 0.0f)
	{
		// No hit, no win
		return false;
	}

	contactPoint = ray.origin + ray.direction * t;
	return true;
}

bool WOWGE::Maths::Intersect(const Ray& ray, const Plane& plane, float& distance)
{
	const float orgDotN = Dot(ray.origin, plane.n);
	const float dirDotN = Dot(ray.direction, plane.n);

	// Check if ray is parallel to the plane
	if (std::abs(dirDotN) < FLT_MIN)
	{ 
		if (std::abs(orgDotN - plane.d) < FLT_MIN)
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Compute distance
	distance = (plane.d - orgDotN) / dirDotN;
	return true;
}

bool WOWGE::Maths::Intersect(const Ray& ray, const Plane& plane, Vector3& contactPoint)
{
	const float orgDotN = Dot(ray.origin, plane.n);
	const float dirDotN = Dot(ray.direction, plane.n);

	float distance{ 0.0f };

	// Check if ray is parallel to the plane
	if (std::abs(dirDotN) < FLT_MIN)
	{
		if (std::abs(orgDotN - plane.d) < FLT_MIN)
		{
			contactPoint = ray.origin;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Compute distance
	distance = (plane.d - orgDotN) / dirDotN;

	if (distance < FLT_MIN)
	{
		return false;
	}
	contactPoint = ray.origin + ray.direction * distance;
	return true;
}

bool WOWGE::Maths::Intersect(const Ray& ray, const AABB& aabb, float& distEntry, Vector3& contactPoint)
{
	// Returns true if ray intersects bounding box
	// Sets d1 to entry distance, d2 to exit distance (along ray.dir)

	Vector3 boxMin = aabb.center - aabb.extend;
	Vector3 boxMax = aabb.center + aabb.extend;
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	float divx = 1.0f / ray.direction.x;
	float divy = 1.0f / ray.direction.y;
	float divz = 1.0f / ray.direction.z;

	if (divx >= 0.0f)
	{
		tmin = (boxMin.x - ray.origin.x) * divx;
		tmax = (boxMax.x - ray.origin.x) * divx;
	}
	else
	{
		tmin = (boxMax.x - ray.origin.x) * divx;
		tmax = (boxMin.x - ray.origin.x) * divx;
	}
	if (divy >= 0.0f)
	{
		tymin = (boxMin.y - ray.origin.y) * divy;
		tymax = (boxMax.y - ray.origin.y) * divy;
	}
	else
	{
		tymin = (boxMax.y - ray.origin.y) * divy;
		tymax = (boxMin.y - ray.origin.y) * divy;
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	if (divz >= 0.0f)
	{
		tzmin = (boxMin.z - ray.origin.z) * divz;
		tzmax = (boxMax.z - ray.origin.z) * divz;
	}
	else
	{
		tzmin = (boxMax.z - ray.origin.z) * divz;
		tzmax = (boxMin.z - ray.origin.z) * divz;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}

	distEntry = tmin;
	//distExit = tmax;
	contactPoint = ray.origin + ray.direction * distEntry;
	return true;
}