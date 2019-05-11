#ifndef INCLUDED_MATH_QUATERNION_H
#define INCLUDED_MATH_QUATERNION_H

#include "Vector3.h"
#include "Matrix44.h"

namespace WOWGE
{
namespace Maths
{
struct Quaternion
{
	//x, y, z is the imaginary component, w is the real component
	float x, y, z, w;

	Quaternion() :x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(float xParam, float yParam, float zParam, float wParam) : x(xParam), y(yParam), z(zParam), w(wParam) {}

	Quaternion operator+(const Quaternion& q) const;
	Quaternion operator-(const Quaternion& q) const;
	Quaternion operator*(float s)             const;
	Quaternion operator/(float s)             const;
	Quaternion operator*(const Quaternion& q) const;
	
	Quaternion operator+=(const Quaternion& q)     ;
	Quaternion operator-=(const Quaternion& q)     ;
	Quaternion operator*=(float s)                 ;
	Quaternion operator/=(float s)                 ;

	bool operator==(const Quaternion& v) { return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w)); }
};

inline Quaternion Conjugate(const Quaternion& q)
{
	return
	{
		-q.x,
		-q.y,
		-q.z,
		 q.w
	};
}

inline float MagnitudeSquare(const Quaternion& q)
{
	return
	{
		(q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w)
	};
}

inline float Magnitude(const Quaternion& q)
{
	return
	{
		sqrtf(MagnitudeSquare(q))
	};
}

inline Quaternion Normalize(const Quaternion& q)
{
	return
	{
		q / Magnitude(q)
	};
}

inline Quaternion Inverse(const Quaternion& q)
{
	if (Magnitude(q) <= FLT_EPSILON)
	{
		return Conjugate(q);
	}

	return
	{
		Conjugate(q) / MagnitudeSquare(q)
	};
}

inline Quaternion Lerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion q = q0 * (1.0f - t) + (q1 * t);
	return Normalize(q);
}

Quaternion QuaternionAngleAxis(const Vector3& axis, float angleInRadians);
Quaternion MatrixToQuaternion(const Matrix44& mat);
Quaternion QuaternionUsingLookAndUpVector(const Vector3& look, const Vector3& up);
Quaternion QuaternionUsingFromAndToVector(const Vector3& from, const Vector3& to);
Matrix44 QuaternionToMatrix(const Quaternion& q);
Quaternion Slerp(const Quaternion& q0Param, const Quaternion& q1Param, float t);

Vector3 ToEulerAngles(const Quaternion& quaternion);
Quaternion ToQuaternion(const Vector3& euler);
} //namespace Maths
} //namespace WOWGE

#endif // !INCLUDED_MATH_QUATERNION_H
