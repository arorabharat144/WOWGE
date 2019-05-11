#ifndef INCLUDED_MATH_VECTOR2_H
#define INCLUDED_MATH_VECTOR2_H

namespace WOWGE
{
namespace Maths
{

struct Vector2
{
	float x, y;

	Vector2() :x(0.0f), y(0.0f) {}
	Vector2(float xParam, float yParam) : x(xParam), y(yParam) {}

	Vector2 operator+(const Vector2& v)const { return { x + v.x, y + v.y}; }
	Vector2 operator-(const Vector2& v)const { return { x - v.x, y - v.y}; }
	Vector2 operator*(float num)const { return { x * num, y * num}; }
	Vector2 operator/(float num)const { return { x / num, y / num}; }

	Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	Vector2& operator*=(float num) { x *= num; y *= num; return *this; }
	Vector2& operator/=(float num) { x /= num; y /= num; return *this; }

	bool operator==(const Vector2& v) { return ((x == v.x) && (y == v.y)); }
	bool operator!=(const Vector2& v) { return ((x != v.x) || (y != v.y)); }
};

inline float Dot(const Vector2& v1, const Vector2& v2) { return ((v1.x * v2.x) + (v1.y * v2.y)); }
inline float MagnitudeSquare(const Vector2& v) { return ((v.x * v.x) + (v.y * v.y)); }
inline float Magnitude(const Vector2& v) { return (sqrtf(MagnitudeSquare(v))); }
inline Vector2 Normalize(const Vector2& v) { return (v / Magnitude(v)); }
inline float DistanceSquare(const Vector2& v1, const Vector2& v2) { return (((v2.x - v1.x)*(v2.x - v1.x)) + ((v2.y - v1.y)*(v2.y - v1.y))); }
inline float Distance(const Vector2& v1, const Vector2& v2) { return (sqrtf(DistanceSquare(v1, v2))); }
inline Vector2 Project(const Vector2& v1, const Vector2& v2) { return { v2 * (Dot(v1,v2) / MagnitudeSquare(v2)) }; }
inline Vector2 Reflect(const Vector2& v, const Vector2& normal) { return (v - (normal*Dot(v, normal)*2.0f)); }

} //Namespace Maths
} //Namespace WOWGE

#endif // !INCLUDED_MATH_VECTOR2_H