#ifndef INCLUDED_MATH_VECTOR4_H
#define INCLUDED_MATH_VECTOR4_H

namespace WOWGE
{
namespace Maths
{
struct Vector4
{
	float x, y, z, w;

	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(float xParam, float yParam, float zParam, float wParam) : x(xParam), y(yParam), z(zParam), w(wParam) {}

	static Vector4 Black() { return Vector4(0.0f, 0.0f, 0.0f, 1.0f); }
	static Vector4 Gray() { return Vector4(0.5f, 0.5f, 0.5f, 1.0f); }
	static Vector4 DarkGray() { return Vector4(0.25f, 0.25f, 0.25f, 1.0f); }
	static Vector4 White() { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
	static Vector4 LightRed() { return Vector4(1.0f, 0.5f, 0.5f, 1.0f); }
	static Vector4 Red() { return Vector4(1.0f, 0.0f, 0.0f, 1.0f); }
	static Vector4 Green() { return Vector4(0.0f, 1.0f, 0.0f, 1.0f); }
	static Vector4 Blue() { return Vector4(0.0f, 0.0f, 1.0f, 1.0f); }
	static Vector4 Yellow() { return Vector4(1.0f, 1.0f, 0.0f, 1.0f); }
	static Vector4 Orange() { return Vector4(1.0f, 0.5f, 0.0f, 1.0f); }
	static Vector4 Cyan() { return Vector4(0.0f, 1.0f, 1.0f, 1.0f); }
	static Vector4 Magenta() { return Vector4(1.0f, 0.0f, 1.0f, 1.0f); }

	Vector4 operator+(const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
	Vector4 operator-(const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vector4 operator*(float num) const { return Vector4(x * num, y * num, z * num, w * num); }
	Vector4 operator/(float num) const { return Vector4(x / num, y / num, z / num, w / num); }

	Vector4& operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	Vector4& operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	Vector4& operator*=(float num) { x *= num; y *= num; z *= num; w *= num; return *this; }
	Vector4& operator/=(float num) { x /= num; y /= num; z /= num; w /= num; return *this; }

	bool operator==(const Vector4& v) { return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w)); }

	bool operator!=(const Vector4& v) { return ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w)); }
};

} //Namespace Maths
} //Namespace WOWGE

#endif // !INCLUDED_MATH_VECTOR4_H
