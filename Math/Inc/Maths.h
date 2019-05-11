#ifndef INCLUDED_MATHS_H
#define INCLUDED_MATHS_H

#include "Common.h"

#include "AABB.h"
#include "Circle.h"
#include "LineSegment.h"
#include "MetaRegistration.h"
#include "Matrix44.h"
#include "Plane.h"
#include "Quaternion.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace
{
	std::random_device randomDevice;
	std::mt19937 engine{ randomDevice() };
}

namespace WOWGE
{
namespace Maths
{

template <typename T> 
inline T Min(T a, T b) { return (a > b) ? b : a; }

template <typename T>
inline T Max(T a, T b) { return (a < b) ? b : a; }

template <typename T>
inline T Clamp(T value, T min, T max) { return Max(min, Min(max, value)); }

inline bool IsZero(float value) { return (value <= FLT_MIN); }

inline Vector3 TransformCoord(const Vector3& v, const Matrix44& m)
{
	return
	{
		v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
		v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
		v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43
	};
}

inline Vector3 TransformNormal(const Vector3& v, const Matrix44& m)
{
	return
	{
		v.x * m.m11 + v.y * m.m21 + v.z * m.m31,
		v.x * m.m12 + v.y * m.m22 + v.z * m.m32,
		v.x * m.m13 + v.y * m.m23 + v.z * m.m33
	};
}

inline Vector3 GetTranslation(const Maths::Matrix44& transform) 
{
	return { transform.m41, transform.m42, transform.m43 };
}

inline Quaternion GetRotation(const Matrix44& matrix)
{
	return Maths::MatrixToQuaternion(matrix);
}

inline Vector3 GetScale(const Matrix44& matrix)
{
	return { Maths::Magnitude({ matrix.m11,  matrix.m12,  matrix.m13 }), Maths::Magnitude({ matrix.m21, matrix.m22, matrix.m23 }), Maths::Magnitude({ matrix.m31,  matrix.m32,  matrix.m33 }) };
}

inline Matrix44 MatrixRotationAxis(const Vector3& axis, float rad)
{
	const Vector3 u = Normalize(axis);
	const float x = u.x;
	const float y = u.y;
	const float z = u.z;
	const float s = sin(rad);
	const float c = cos(rad);

	return
	{
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	};
}

inline float ConvertDegreeToRadian(float degree)
{
	return degree * (3.14f / 180.0f);
}

inline float ConvertRadianToDegree(float radian)
{
	return radian * (180.0f / 3.14f);
}

inline Vector3 GetRight(const Maths::Matrix44& transform) { return { transform.m11, transform.m12, transform.m13 }; }
inline Vector3 GetUp(const Maths::Matrix44& transform) { return { transform.m21, transform.m22, transform.m23 }; }
inline Vector3 GetForward(const Maths::Matrix44& transform) { return { transform.m31, transform.m32, transform.m33 }; }

inline float Absolute(float value)
{
	float number = value;
	if (value < 0.0f)
	{
		number = -value;
	}
	return number;
}

inline float RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{min, max}(engine);
}

inline int RandomInt(int min, int max)
{
	return std::uniform_int_distribution<int>{min, max}(engine);
}

} //namespace Maths
} //namespace WOWGE

#endif // !INCLUDED_MATHS_H
