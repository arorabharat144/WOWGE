#ifndef INCLUDED_MATH_MATRIX44_H
#define INCLUDED_MATH_MATRIX44_H

namespace WOWGE
{
namespace Maths
{
struct Matrix44
{
	float m11, m12, m13, m14,
		  m21, m22, m23, m24,
		  m31, m32, m33, m34,
		  m41, m42, m43, m44;

	Matrix44() :
		m11(1.0f), m12(0.0f), m13(0.0f), m14(0.0f),
		m21(0.0f), m22(1.0f), m23(0.0f), m24(0.0f),
		m31(0.0f), m32(0.0f), m33(1.0f), m34(0.0f),
		m41(0.0f), m42(0.0f), m43(0.0f), m44(1.0f) {}

	Matrix44(
		float param11, float param12, float param13, float param14,
		float param21, float param22, float param23, float param24,
		float param31, float param32, float param33, float param34,
		float param41, float param42, float param43, float param44):
		m11(param11), m12(param12), m13(param13), m14(param14),
		m21(param21), m22(param22), m23(param23), m24(param24),
		m31(param31), m32(param32), m33(param33), m34(param34),
		m41(param41), m42(param42), m43(param43), m44(param44) {}
	//NO VECTOR PARAMETERS
	static Matrix44 Zero()
	{ 
		return
		{
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		};
	}
	static Matrix44 Identity()
	{
		return 
		{ 
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f 
		}; 
	}
	static Matrix44 Translation(float x, float y, float z)
	{ 
		return
		{ 
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x, y, z, 1.0f 
		};
	}
	static Matrix44 Translation(const Vector3& v)
	{
		return
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			v.x, v.y, v.z, 1.0f
		};
	}

	static Matrix44 RotationX(float angleInRadians)
	{ 
		return
		{ 
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosf(angleInRadians), sinf(angleInRadians), 0.0f,
			0.0f, -sinf(angleInRadians), cosf(angleInRadians), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		}; 
	}
	static Matrix44 RotationY(float angleInRadians)
	{ 
		return
		{
			cosf(angleInRadians), 0.0f, -sinf(angleInRadians), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sinf(angleInRadians), 0.0f, cosf(angleInRadians), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f 
		}; 
	}
	static Matrix44 RotationZ(float angleInRadians)
	{ 
		return
		{ 
			cosf(angleInRadians), sinf(angleInRadians), 0.0f, 0.0f,
			-sinf(angleInRadians), cosf(angleInRadians), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f 
		}; 
	}
	static Matrix44 Scaling(float s) 
	{
		return 
		{ 
			s, 0.0f, 0.0f, 0.0f,
			0.0f, s, 0.0f, 0.0f,
			0.0f, 0.0f, s, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		}; 
	}
	static Matrix44 Scaling(float sx, float sy, float sz)
	{ 
		return 
		{ 
			sx, 0.0f, 0.0f, 0.0f,
			0.0f, sy, 0.0f, 0.0f,
			0.0f, 0.0f, sz, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	static Matrix44 Scaling(const Vector3& v)
	{
		return
		{
			v.x, 0.0f, 0.0f, 0.0f,
			0.0f, v.y, 0.0f, 0.0f,
			0.0f, 0.0f, v.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	Matrix44 operator+(const Matrix44& m)const 
	{
		return
		{
			m11 + m.m11,m12 + m.m12,m13 + m.m13,m14 + m.m14,
			m21 + m.m21,m22 + m.m22,m23 + m.m23,m24 + m.m24,
			m31 + m.m31,m32 + m.m32,m33 + m.m33,m34 + m.m34,
			m41 + m.m41,m42 + m.m42,m43 + m.m43,m44 + m.m44
		};
	}

	Matrix44 operator-(const Matrix44& m)const
	{
		return
		{
			m11 - m.m11,m12 - m.m12,m13 - m.m13,m14 - m.m14,
			m21 - m.m21,m22 - m.m22,m23 - m.m23,m24 - m.m24,
			m31 - m.m31,m32 - m.m32,m33 - m.m33,m34 - m.m34,
			m41 - m.m41,m42 - m.m42,m43 - m.m43,m44 - m.m44
		};
	}

	Matrix44 operator*(const Matrix44& m)const
	{
		return
		{
			(m11 * m.m11) + (m12 * m.m21) + (m13 * m.m31) + (m14 * m.m41),
			(m11 * m.m12) + (m12 * m.m22) + (m13 * m.m32) + (m14 * m.m42),
			(m11 * m.m13) + (m12 * m.m23) + (m13 * m.m33) + (m14 * m.m43),
			(m11 * m.m14) + (m12 * m.m24) + (m13 * m.m34) + (m14 * m.m44),

			(m21 * m.m11) + (m22 * m.m21) + (m23 * m.m31) + (m24 * m.m41),
			(m21 * m.m12) + (m22 * m.m22) + (m23 * m.m32) + (m24 * m.m42),
			(m21 * m.m13) + (m22 * m.m23) + (m23 * m.m33) + (m24 * m.m43),
			(m21 * m.m14) + (m22 * m.m24) + (m23 * m.m34) + (m24 * m.m44),

			(m31 * m.m11) + (m32 * m.m21) + (m33 * m.m31) + (m34 * m.m41),
			(m31 * m.m12) + (m32 * m.m22) + (m33 * m.m32) + (m34 * m.m42),
			(m31 * m.m13) + (m32 * m.m23) + (m33 * m.m33) + (m34 * m.m43),
			(m31 * m.m14) + (m32 * m.m24) + (m33 * m.m34) + (m34 * m.m44),

			(m41 * m.m11) + (m42 * m.m21) + (m43 * m.m31) + (m44 * m.m41),
			(m41 * m.m12) + (m42 * m.m22) + (m43 * m.m32) + (m44 * m.m42),
			(m41 * m.m13) + (m42 * m.m23) + (m43 * m.m33) + (m44 * m.m43),
			(m41 * m.m14) + (m42 * m.m24) + (m43 * m.m34) + (m44 * m.m44)
		};
	}

	Matrix44 operator*(float val)const
	{
		return
		{
			m11 * val, m12 * val, m13 * val, m14 * val,
			m21 * val, m22 * val, m23 * val, m24 * val,
			m31 * val, m32 * val, m33 * val, m34 * val,
			m41 * val, m42 * val, m43 * val, m44 * val
		};
	}

	Matrix44 operator/(float val)const
	{
		return
		{
			m11 / val, m12 / val, m13 / val, m14 / val,
			m21 / val, m22 / val, m23 / val, m24 / val,
			m31 / val, m32 / val, m33 / val, m34 / val,
			m41 / val, m42 / val, m43 / val, m44 / val
		};
	}

	Matrix44 operator+=(const Matrix44& m)
	{
			m11 += m.m11; m12 += m.m12; m13 += m.m13; m14 += m.m14;
			m21 += m.m21; m22 += m.m22; m23 += m.m23; m24 += m.m24;
			m31 += m.m31; m32 += m.m32; m33 += m.m33; m34 += m.m34;
			m41 += m.m41; m42 += m.m42; m43 += m.m43; m44 += m.m44;
			return *this;
	}

	Matrix44 operator-=(const Matrix44& m)
	{
		m11 -= m.m11; m12 -= m.m12; m13 -= m.m13; m14 -= m.m14;
		m21 -= m.m21; m22 -= m.m22; m23 -= m.m23; m24 -= m.m24;
		m31 -= m.m31; m32 -= m.m32; m33 -= m.m33; m34 -= m.m34;
		m41 -= m.m41; m42 -= m.m42; m43 -= m.m43; m44 -= m.m44;
		return *this;
	}

	Matrix44 operator*=(float val)
	{
		m11 *= val; m12 *= val; m13 *= val; m14 *= val;
		m21 *= val; m22 *= val; m23 *= val; m24 *= val;
		m31 *= val; m32 *= val; m33 *= val; m34 *= val;
		m41 *= val; m42 *= val; m43 *= val; m44 *= val;
		return *this;
	}

	Matrix44 operator/=(float val)
	{
		m11 /= val; m12 /= val; m13 /= val; m14 /= val;
		m21 /= val; m22 /= val; m23 /= val; m24 /= val;
		m31 /= val; m32 /= val; m33 /= val; m34 /= val;
		m41 /= val; m42 /= val; m43 /= val; m44 /= val;
		return *this;
	}


	bool operator==(const Matrix44& matrix)
	{
		if (m11 == matrix.m11&& m12 == matrix.m12&& m13 == matrix.m13 && m14 == matrix.m14 &&
			m21 == matrix.m21&& m22 == matrix.m22&& m23 == matrix.m23&& m24 == matrix.m24&&
			m31 == matrix.m31&& m32 == matrix.m32&& m33 == matrix.m33&& m34 == matrix.m34&&
			m41 == matrix.m41&& m42 == matrix.m42&& m43 == matrix.m43&& m44 == matrix.m44)
		{
			return true;
		}
		return false;
	}
};

inline float Determinant(const Matrix44& m)
{
	float det = 0.0f;
	det =  (m.m11 * (m.m22 * (m.m33 * m.m44 - (m.m43 * m.m34)) - m.m23 * (m.m32 * m.m44 - (m.m42 * m.m34)) + m.m24 * (m.m32 * m.m43 - (m.m42 * m.m33))));
	det -= (m.m12 * (m.m21 * (m.m33 * m.m44 - (m.m43 * m.m34)) - m.m23 * (m.m31 * m.m44 - (m.m41 * m.m34)) + m.m24 * (m.m31 * m.m43 - (m.m41 * m.m33))));
	det += (m.m13 * (m.m21 * (m.m32 * m.m44 - (m.m42 * m.m34)) - m.m22 * (m.m31 * m.m44 - (m.m41 * m.m34)) + m.m24 * (m.m31 * m.m42 - (m.m41 * m.m32))));
	det -= (m.m14 * (m.m21 * (m.m32 * m.m43 - (m.m42 * m.m33)) - m.m22 * (m.m31 * m.m43 - (m.m41 * m.m33)) + m.m23 * (m.m31 * m.m42 - (m.m41 * m.m32))));
	return det;
}

inline Matrix44 Adjoint(const Matrix44& m)
{
	return 
	{
		 (m.m22 * ((m.m33 * m.m44) - (m.m43 * m.m34)) - m.m23 * ((m.m32 * m.m44) - (m.m42 * m.m34)) + m.m24 * ((m.m32 * m.m43) - (m.m42 * m.m33))),
		-(m.m12 * ((m.m33 * m.m44) - (m.m43 * m.m34)) - m.m13 * ((m.m32 * m.m44) - (m.m42 * m.m34)) + m.m14 * ((m.m32 * m.m43) - (m.m42 * m.m33))),
		 (m.m12 * ((m.m23 * m.m44) - (m.m43 * m.m24)) - m.m13 * ((m.m22 * m.m44) - (m.m42 * m.m24)) + m.m14 * ((m.m22 * m.m43) - (m.m42 * m.m23))),
		-(m.m12 * ((m.m23 * m.m34) - (m.m33 * m.m24)) - m.m13 * ((m.m22 * m.m34) - (m.m32 * m.m24)) + m.m14 * ((m.m22 * m.m33) - (m.m32 * m.m23))),

		-(m.m21 * ((m.m33 * m.m44) - (m.m43 * m.m34)) - m.m31 * ((m.m23 * m.m44) - (m.m24 * m.m43)) + m.m41 * ((m.m23 * m.m34) - (m.m24 * m.m33))),
		 (m.m11 * ((m.m33 * m.m44) - (m.m43 * m.m34)) - m.m13 * ((m.m31 * m.m44) - (m.m41 * m.m34)) + m.m14 * ((m.m31 * m.m43) - (m.m41 * m.m33))),
		-(m.m11 * ((m.m23 * m.m44) - (m.m43 * m.m24)) - m.m13 * ((m.m21 * m.m44) - (m.m41 * m.m24)) + m.m14 * ((m.m21 * m.m43) - (m.m41 * m.m23))),
		 (m.m11 * ((m.m23 * m.m34) - (m.m33 * m.m24)) - m.m13 * ((m.m21 * m.m34) - (m.m31 * m.m24)) + m.m14 * ((m.m21 * m.m33) - (m.m31 * m.m23))),

		 (m.m21 * ((m.m32 * m.m44) - (m.m42 * m.m34)) - m.m31 * ((m.m22 * m.m44) - (m.m42 * m.m24)) + m.m41 * ((m.m22 * m.m34) - (m.m32 * m.m24))),
		-(m.m11 * ((m.m32 * m.m44) - (m.m42 * m.m34)) - m.m31 * ((m.m12 * m.m44) - (m.m42 * m.m14)) + m.m41 * ((m.m12 * m.m34) - (m.m32 * m.m14))),
		 (m.m11 * ((m.m22 * m.m44) - (m.m42 * m.m24)) - m.m12 * ((m.m21 * m.m44) - (m.m41 * m.m24)) + m.m14 * ((m.m21 * m.m42) - (m.m41 * m.m22))),
		-(m.m11 * ((m.m22 * m.m34) - (m.m32 * m.m24)) - m.m21 * ((m.m12 * m.m34) - (m.m32 * m.m14)) + m.m31 * ((m.m12 * m.m24) - (m.m22 * m.m14))),

		-(m.m21 * ((m.m32 * m.m43) - (m.m42 * m.m33)) - m.m31 * ((m.m22 * m.m43) - (m.m42 * m.m23)) + m.m41 * ((m.m22 * m.m33) - (m.m32 * m.m23))),
		 (m.m11 * ((m.m32 * m.m43) - (m.m42 * m.m33)) - m.m12 * ((m.m31 * m.m43) - (m.m41 * m.m33)) + m.m13 * ((m.m31 * m.m42) - (m.m41 * m.m32))),
		-(m.m11 * ((m.m22 * m.m43) - (m.m42 * m.m23)) - m.m12 * ((m.m21 * m.m43) - (m.m41 * m.m23)) + m.m13 * ((m.m21 * m.m42) - (m.m41 * m.m22))),
		 (m.m11 * ((m.m22 * m.m33) - (m.m32 * m.m23)) - m.m12 * ((m.m21 * m.m33) - (m.m31 * m.m23)) + m.m13 * ((m.m21 * m.m32) - (m.m31 * m.m22)))
	};
}

inline Matrix44 Inverse(const Matrix44& m)
{
	const float determinant = Determinant(m);
	const float invDet = 1.0f / determinant;
	return Adjoint(m) * invDet;
}

inline Matrix44 Transpose(const Matrix44& m)
{
	return
	{
		m.m11, m.m21, m.m31, m.m41,
		m.m12, m.m22, m.m32, m.m42,
		m.m13, m.m23, m.m33, m.m43,
		m.m14, m.m24, m.m34, m.m44
	};
}



} //Namespace Maths
} //Namespace WOWGE

#endif // !INCLUDED_MATH_MATRIX44_H
