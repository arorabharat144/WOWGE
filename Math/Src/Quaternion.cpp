#include "Precompiled.h"
#include "Quaternion.h"

using namespace WOWGE;
using namespace WOWGE::Maths;

Quaternion Quaternion::operator+(const Quaternion& q) const 
{ 
	return 
	{
		x + q.x, y + q.y, z + q.z, w + q.w
	}; 
}

Quaternion Quaternion::operator-(const Quaternion& q) const
{ 
	return 
	{
		x - q.x, y - q.y, z - q.z, w - q.w 
	};
}

Quaternion Quaternion::operator*(float s) const 
{
	return
	{ 
		x * s  , y * s  , z * s  , w * s 
	};
}

Quaternion Quaternion::operator/(float s) const 
{ 
	return 
	{
		x / s  , y / s  , z / s  , w / s
	};
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	return
	{
		x * q.w + y * q.z - z * q.y + w * q.x,
		-x * q.z + y * q.w + z * q.x + w * q.y,
		x * q.y - y * q.x + z * q.w + w * q.z,
		-x * q.x - y * q.y - z * q.z + w * q.w
	};
}

Quaternion Quaternion::operator+=(const Quaternion& q)
{ 
	x += q.x; y += q.y; z += q.z; w += q.w;
	return *this;
}

Quaternion Quaternion::operator-=(const Quaternion& q)
{ 
	x -= q.x; y -= q.y; z -= q.z; w -= q.w; 
	return *this;
}

Quaternion Quaternion::operator*=(float s) 
{
	x *= s; y *= s; z *= s; w *= s;
	return *this;
}
Quaternion Quaternion::operator/=(float s)
{
	x /= s; y /= s; z /= s; w /= s;
	return *this;
}

Quaternion WOWGE::Maths::QuaternionAngleAxis(const Vector3& axis, float angleInRadians)
{
	float c = cosf(angleInRadians * 0.5f);
	float s = sinf(angleInRadians * 0.5f);
	Vector3 normalizedAxis = Maths::Normalize(axis);

	return Maths::Normalize({ normalizedAxis.x * s, normalizedAxis.y * s, normalizedAxis.z * s, c });

}

Quaternion WOWGE::Maths::MatrixToQuaternion(const Matrix44& mat)
{ 
	Quaternion quaternion;
	quaternion.w = sqrt(std::max(0.0f, 1.0f + mat.m11 + mat.m22 + mat.m33)) * 0.5f;
	quaternion.x = sqrt(std::max(0.0f, 1.0f + mat.m11 - mat.m22 - mat.m33)) * 0.5f;
	quaternion.y = sqrt(std::max(0.0f, 1.0f - mat.m11 + mat.m22 - mat.m33)) * 0.5f;
	quaternion.z = sqrt(std::max(0.0f, 1.0f - mat.m11 - mat.m22 + mat.m33)) * 0.5f;

	return
	{
		copysign(quaternion.x, mat.m23 - mat.m32),
		copysign(quaternion.y, mat.m31 - mat.m13),
		copysign(quaternion.z, mat.m12 - mat.m21),
		quaternion.w
	};
}

Quaternion WOWGE::Maths::QuaternionUsingLookAndUpVector(const Vector3& look, const Vector3& up)
{
	Vector3 z = Maths::Normalize(look);
	Vector3 x = Maths::Normalize(Maths::Cross(up, z));
	Vector3 y = Maths::Normalize(Maths::Cross(z, x));

	Matrix44 mat
	{
		x.x,  x.y,  x.z,  0.0f,
		y.x,  y.y,  y.z,  0.0f,
		z.x,  z.y,  z.z,  0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return MatrixToQuaternion(mat);
}

Quaternion WOWGE::Maths::QuaternionUsingFromAndToVector(const Vector3& from, const Vector3& to)
{
	Quaternion q;
	Vector3 a = (Maths::Cross(from, to));
	
	q.x = a.x;
	q.y = a.y;
	q.z = a.z;
	q.w = sqrtf((Maths::MagnitudeSquare(from) * (Maths::MagnitudeSquare(to))) + Maths::Dot(from, to));

	return Normalize(q);
}

Matrix44 WOWGE::Maths::QuaternionToMatrix(const Quaternion& q)
{
	return
	{
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
		0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	};
}

Quaternion WOWGE::Maths::Slerp(const Quaternion& q0Param, const Quaternion& q1Param, float t)
{
	Quaternion q0 { Maths::Normalize(q0Param) };
	Quaternion q1 { Maths::Normalize(q1Param) };

	if (q0 == q1)
	{
		return q0;
	}

	//Find the dot product
	float dot = (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z) + (q0.w * q1.w);

	//Determine the direction of the rotation
	if (dot < 0.0f)
	{
		dot = -dot;
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
		q1.w = -q1.w;
	}
	else if (dot > 0.9995f)
	{
		return (Lerp(q0, q1, t));
	}

	float theta_0 = acos(dot);
	float theta = theta_0 * t;
	float sinTheta = sin(theta);
	float sinTheta_0 = sin(theta_0);

	float s0 = cos(theta) - dot * sinTheta / sinTheta_0;
	float s1 = sinTheta / sinTheta_0;

	return (q0 * s0) + (q1 * s1);

	//float theta = acos(dot);
	//float sinTheta = sin(theta);
	//float scale0 = sin(theta * (1.0f - t)) / sinTheta;
	//float scale1 = sin(theta * t) / sinTheta;
	//
	////Perform the slerp
	//return
	//{
	//	(q0.x * scale0) + (q1.x * scale1),
	//	(q0.y * scale0) + (q1.y * scale1),
	//	(q0.z * scale0) + (q1.z * scale1),
	//	(q0.w * scale0) + (q1.z * scale1)
	//};
}

Vector3 WOWGE::Maths::ToEulerAngles(const Quaternion& q)
{
	Quaternion quaternion = Maths::Normalize(q);

	//X-Axis Rotation (Pitch)
	float sinX = 2.0f * ((quaternion.w * quaternion.y) - (quaternion.z * quaternion.x));
	float rotationX;
	if (fabs(sinX) >= 1.0f)
	{
		rotationX = copysign(3.141592653589793238462643f * 0.5f, sinX);
	}
	else
	{
		rotationX = asin(sinX);
	}

	//Y-Axis Rotation (Yaw)
	float sinY = 2.0f*((quaternion.w * quaternion.z) + (quaternion.x * quaternion.y));
	float cosY = 1.0f - 2.0f * ((quaternion.y * quaternion.y) + (quaternion.z * quaternion.z));
	float rotationY = atan2(sinY, cosY);

	//Z-Axis Rotation (Roll)
	float sinZ = 2.0f * ((quaternion.w * quaternion.x) + (quaternion.y * quaternion.z));
	float cosZ = 1.0f - 2.0f * ((quaternion.x * quaternion.x) + (quaternion.y * quaternion.y));
	float rotationZ = atan2(sinZ, cosZ);

	return { rotationX, rotationY, rotationZ };
}

Quaternion WOWGE::Maths::ToQuaternion(const Vector3& euler)
{
	Quaternion quaternion;

	float cosX = cos(euler.x * 0.5f);
	float sinX = sin(euler.x * 0.5f);
	float cosY = cos(euler.y * 0.5f);
	float sinY = sin(euler.y * 0.5f);
	float cosZ = cos(euler.z * 0.5f);
	float sinZ = sin(euler.z * 0.5f);

	quaternion.w = (cosY * cosZ * cosX) + (sinY * sinZ * sinX);
	quaternion.x = (cosY * cosZ * sinX) + (sinY * sinZ * cosX);
	quaternion.y = (sinY * cosZ * cosX) - (cosY * sinZ * sinX);
	quaternion.z = (cosY * sinZ * cosX) - (sinY * cosZ * sinX);

	return quaternion;
}