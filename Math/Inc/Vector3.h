#ifndef INCLUDED_MATH_VECTOR3_H
#define INCLUDED_MATH_VECTOR3_H

namespace WOWGE
{
namespace Maths
{

struct Vector3
{
	float x, y, z;

	Vector3() :x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float xParam, float yParam, float zParam) : x(xParam), y(yParam), z(zParam) {}

	Vector3 operator+(const Vector3& v)const { return { x + v.x, y + v.y, z + v.z }; }
	Vector3 operator-(const Vector3& v)const { return { x - v.x, y - v.y, z - v.z }; }
	Vector3 operator*(float num)const { return { x * num, y * num, z * num }; }
	Vector3 operator/(float num)const { return { x / num, y / num, z / num }; }

	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(float num) { x *= num; y *= num; z *= num; return *this; }
	Vector3& operator/=(float num) { x /= num; y /= num; z /= num; return *this; }

	bool operator==(const Vector3& v) { return ((x == v.x) && (y == v.y) && (z == v.z)); }
	bool operator!=(const Vector3& v) { return ((x != v.x) || (y != v.y) || (z != v.z)); }
	
};

inline Vector3 Vector3Min(const Maths::Vector3& v1, const Maths::Vector3& v2)
{ 
	Maths::Vector3 returnVector;
	if (v1.x > v2.x)
	{
		returnVector.x = v2.x;
	}
	else
	{
		returnVector.x = v1.x;
	}

	if (v1.y > v2.y)
	{
		returnVector.y = v2.y;
	}
	else
	{
		returnVector.y = v1.y;
	}

	if (v1.z > v2.z)
	{
		returnVector.z = v2.z;
	}
	else
	{
		returnVector.z = v1.z;
	}
	
	return returnVector;
}

inline Vector3 Vector3Max(const Maths::Vector3& v1, const Maths::Vector3& v2)
{
	Maths::Vector3 returnVector;
	if (v1.x < v2.x)
	{
		returnVector.x = v2.x;
	}
	else
	{
		returnVector.x = v1.x;
	}

	if (v1.y < v2.y)
	{
		returnVector.y = v2.y;
	}
	else
	{
		returnVector.y = v1.y;
	}

	if (v1.z < v2.z)
	{
		returnVector.z = v2.z;
	}
	else
	{
		returnVector.z = v1.z;
	}

	return returnVector;
}

inline float Dot(const Vector3& v1, const Vector3& v2) { return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z)); }


inline Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t) { return v0 + ((v1 - v0) * t); }

inline float MagnitudeSquare(const Vector3& v) { return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }
inline float MagnitudeXZSquare(const Vector3& v) { return ((v.x * v.x) + (v.z * v.z)); }
inline float MagnitudeXZ(const Vector3& v) { return sqrtf((v.x * v.x) + (v.z * v.z)); }
inline float Magnitude(const Vector3& v) { return (sqrtf(MagnitudeSquare(v))); }
inline Vector3 Normalize(const Vector3& v) { /*ASSERT(Magnitude(v) > 0.000001f, "")*/;  return (v / Magnitude(v)); }

inline Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	ASSERT(Dot(Normalize(v1), Normalize(v2)) < 1.0f, "[Maths] Cannot cross two vectors that are parallel");
	return{ (v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x) };
}
inline float DistanceSquare(const Vector3& v1, const Vector3& v2) { return (((v2.x - v1.x)*(v2.x - v1.x)) + ((v2.y - v1.y)*(v2.y - v1.y)) + ((v2.z - v1.z)*(v2.z - v1.z))); }
inline float Distance(const Vector3& v1, const Vector3& v2) { return (sqrtf(DistanceSquare(v1, v2))); }
inline Vector3 Project(const Vector3& v1, const Vector3& v2) { return { v2 * (Dot(v1,v2) / MagnitudeSquare(v2)) }; }
inline Vector3 Reflect(const Vector3& v, const Vector3& normal) { return (v - (normal * Dot(v, normal) * 2.0f)); }

} //Namespace Maths
} //Namespace WOWGE

#endif // !INCLUDED_MATH_VECTOR3_H
