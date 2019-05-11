#include "Precompiled.h"
#include "AABB.h"
#include "Sphere.h"

using namespace WOWGE;
using namespace WOWGE::Maths;

bool WOWGE::Maths::Intersect(float rectMin, float rectMax, float lineStart, float lineEnd, float& enter, float& exit)
{
	float lineDirection{ lineEnd - lineStart };

	if (abs(lineDirection) < FLT_EPSILON)
	{
		if (lineStart<rectMin || lineStart>rectMax)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	float tsenter{ (rectMin - lineStart) / lineDirection };
	float tsexit{ (rectMax - lineStart) / lineDirection };

	if (tsenter > tsexit)
	{
		std::swap(tsenter, tsexit);
	}

	if (enter > tsexit || tsenter > exit)
	{
		return false;
	}
	else
	{
		enter = std::max(enter, tsenter);
		exit = std::min(exit, tsexit);
		return true;
	}
}

bool WOWGE::Maths::Intersect(const Vector3& point1, const Vector3& point2, const AABB& aabb)
{
	float tenter{ 0.0f };
	float texit{ 1.0f };

	if (!Intersect(aabb.GetMin().x, aabb.GetMax().x, point1.x, point2.x, tenter, texit))
	{
		return false;
	}

	if (!Intersect(aabb.GetMin().y, aabb.GetMax().y, point1.y, point2.y, tenter, texit))
	{
		return false;
	}

	if (!Intersect(aabb.GetMin().z, aabb.GetMax().z, point1.z, point2.z, tenter, texit))
	{
		return false;
	}

	return true;
}

bool WOWGE::Maths::Intersect(const Vector3& point1, const Vector3& point2, const AABB& aabb, float& distance, Vector3& contactPoint)
{
	float tenter{ 0.0f };
	float texit{ 1.0f };

	if (!Intersect(aabb.GetMin().x, aabb.GetMax().x, point1.x, point2.x, tenter, texit))
	{
		return false;
	}

	if (!Intersect(aabb.GetMin().y, aabb.GetMax().y, point1.y, point2.y, tenter, texit))
	{
		return false;
	}

	if (!Intersect(aabb.GetMin().z, aabb.GetMax().z, point1.z, point2.z, tenter, texit))
	{
		return false;
	}

	Vector3 temp{ point2 - point1 };
	contactPoint = point1 + temp * tenter;

	distance = tenter;

	return true;
}

bool WOWGE::Maths::Intersect(const Vector3& point, const AABB& aabb)
{
	const Vector3 test{ point - aabb.center };
	if (abs(test.x) > aabb.extend.x)
	{
		return false;
	}
	if (abs(test.y) > aabb.extend.y)
	{
		return false;
	}
	if (abs(test.z) > aabb.extend.z)
	{
		return false;
	}

	return true;
}

bool WOWGE::Maths::Intersect(const Sphere& sphere, const AABB& aabb)
{
	auto check = [&](
		const float pn,
		const float bmin,
		const float bmax) -> float
	{
		float out = 0;
		float v = pn;

		if (v < bmin)
		{
			float val = (bmin - v);
			out += val * val;
		}

		if (v > bmax)
		{
			float val = (v - bmax);
			out += val * val;
		}

		return out;
	};

	// Squared distance
	float sq = 0.0;

	sq += check(sphere.center.x, aabb.GetMin().x, aabb.GetMax().x);
	sq += check(sphere.center.y, aabb.GetMin().y, aabb.GetMax().y);
	sq += check(sphere.center.z, aabb.GetMin().z, aabb.GetMax().z);

	if (sq <= (sphere.radius * sphere.radius))
	{
		return true;
	}

	return false;
}