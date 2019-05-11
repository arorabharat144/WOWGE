#ifndef INCLUDED_MATH_OBB_H
#define INCLUDED_MATH_OBB_H

#include "Vector3.h"
#include "Quaternion.h"

namespace WOWGE
{
namespace Maths
{
struct OBB
{
	OBB() {}
	OBB(const Maths::Vector3& centerParam, const Maths::Vector3& extendParam) :
		center(centerParam),
		extend(extendParam) {}

	Maths::Vector3 center;
	Maths::Vector3 extend;
	Maths::Quaternion orientation;
};

bool Intersect(const Vector3& point, const OBB& obb);

} //Namespace Maths
} //Namespace WOWGE

#endif // !INCLUDED_MATH_AABB_H
