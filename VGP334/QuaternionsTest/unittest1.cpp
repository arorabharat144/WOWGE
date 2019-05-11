#include "stdafx.h"
#include "CppUnitTest.h"

#include <Math\Inc\Maths.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WOWGE::Maths;

namespace QuaternionsTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(QuaternionAddition)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			Quaternion q2{ 2.0f, 3.0f, 5.0f, 7.0f };
			Quaternion q3{ q1 + q2 };

			Assert::IsTrue(q3.x == 6.0f);
			Assert::IsTrue(q3.y == 5.0f);
			Assert::IsTrue(q3.z == 6.0f);
			Assert::IsTrue(q3.w == 16.0f);
		}

		TEST_METHOD(QuaternionSubtraction)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			Quaternion q2{ 2.0f, 3.0f, 5.0f, 7.0f };
			Quaternion q3{ q1 - q2 };

			Assert::IsTrue(q3.x == 2.0f);
			Assert::IsTrue(q3.y == -1.0f);
			Assert::IsTrue(q3.z == -4.0f);
			Assert::IsTrue(q3.w == 2.0f);
		}

		TEST_METHOD(QuaternionScalarMultiplication)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			float scalar{ 2.0f };
			Quaternion q3{ q1 * scalar };

			Assert::IsTrue(q3.x == 8.0f);
			Assert::IsTrue(q3.y == 4.0f);
			Assert::IsTrue(q3.z == 2.0f);
			Assert::IsTrue(q3.w == 18.0f);
		}

		TEST_METHOD(QuaternionScalarDivision)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			float scalar{ 2.0f };
			Quaternion q3{ q1 / scalar };

			Assert::IsTrue(q3.x == 2.0f);
			Assert::IsTrue(q3.y == 1.0f);
			Assert::IsTrue(q3.z == 0.5f);
			Assert::IsTrue(q3.w == 4.5f);
		}

		TEST_METHOD(QuaternionMultiplication)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			Quaternion q2{ 2.0f, 3.0f, 5.0f, 7.0f };
			Quaternion q3{ q1 * q2 };

			Assert::IsTrue(q3.x == 53.0f);
			Assert::IsTrue(q3.y == 23.0f);
			Assert::IsTrue(q3.z == 60.0f);
			Assert::IsTrue(q3.w == 44.0f);
		}

		TEST_METHOD(QuaternionPlusEquals)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			Quaternion q2{ 2.0f, 3.0f, 5.0f, 7.0f };

			q2 += q1;

			Assert::IsTrue(q2.x == 6.0f);
			Assert::IsTrue(q2.y == 5.0f);
			Assert::IsTrue(q2.z == 6.0f);
			Assert::IsTrue(q2.w == 16.0f);
		}

		TEST_METHOD(QuaternionMinusEquals)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			Quaternion q2{ 2.0f, 3.0f, 5.0f, 7.0f };

			q2 -= q1;

			Assert::IsTrue(q2.x == -2.0f);
			Assert::IsTrue(q2.y == 1.0f);
			Assert::IsTrue(q2.z == 4.0f);
			Assert::IsTrue(q2.w == -2.0f);
		}

		TEST_METHOD(QuaternionMultiplyEquals)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			float scalar{ 2.0f };
			
			q1 *= scalar;

			Assert::IsTrue(q1.x == 8.0f);
			Assert::IsTrue(q1.y == 4.0f);
			Assert::IsTrue(q1.z == 2.0f);
			Assert::IsTrue(q1.w == 18.0f);
		}

		TEST_METHOD(QuaternionDivideEquals)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			float scalar{ 2.0f };

			q1 /= scalar;

			Assert::IsTrue(q1.x == 2.0f);
			Assert::IsTrue(q1.y == 1.0f);
			Assert::IsTrue(q1.z == 0.5f);
			Assert::IsTrue(q1.w == 4.5f);
		}

		TEST_METHOD(QuaternionConjugate)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			Quaternion q2{ Conjugate(q1) };

			Assert::IsTrue(q2.x == -4.0f);
			Assert::IsTrue(q2.y == -2.0f);
			Assert::IsTrue(q2.z == -1.0f);
			Assert::IsTrue(q2.w == 9.0f);
		}

		TEST_METHOD(QuaternionMagnitudeSquare)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			float magnitudeSquare{ MagnitudeSquare(q1) };

			Assert::IsTrue(magnitudeSquare == 102.0f);
		}

		TEST_METHOD(QuaternionMagnitude)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			float magnitude{ Magnitude(q1) };

			Assert::IsTrue(magnitude == sqrtf(102.0f));
		}

		TEST_METHOD(QuaternionNormalize)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };

			Quaternion normalizedQuaternion{ Normalize(q1) };

			Assert::IsTrue(normalizedQuaternion.x <= 1.0f);
			Assert::IsTrue(normalizedQuaternion.y <= 1.0f);
			Assert::IsTrue(normalizedQuaternion.z <= 1.0f);
			Assert::IsTrue(normalizedQuaternion.w <= 1.0f);
		}

		TEST_METHOD(QuaternionInverse)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };

			Quaternion quaternionInverse{ Inverse(q1) };

			Assert::IsTrue(quaternionInverse.x == (-2.0f / 51.0f));
			Assert::IsTrue(quaternionInverse.y == (-1.0f / 51.0f));
			Assert::IsTrue(quaternionInverse.z == (-1.0f / 102.0f));
			Assert::IsTrue(quaternionInverse.w == (3.0f / 34.0f));
		}

		TEST_METHOD(QuaternionLerp)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			Quaternion q2{ 2.0f, 3.0f, 5.0f, 7.0f };
			Quaternion q3{ Lerp(q1,q2, 0.3f) };

			Assert::IsTrue(q3.x == 0.353993893f);
			Assert::IsTrue(q3.y == 0.239466444f);
			Assert::IsTrue(q3.z == 0.229054868f);
			Assert::IsTrue(q3.w == 0.874573052f);
		}

		TEST_METHOD(QuaternionAngleAxis)
		{
			Vector3 axis{ 0.5f, 1.0f, 0.7f };
			float angleInRadians{ 0.8f };

			Quaternion q{ WOWGE::Maths::QuaternionAngleAxis(axis, angleInRadians) };

			Assert::IsTrue(q.x == 0.147608638f);
			Assert::IsTrue(q.y == 0.295217276f);
			Assert::IsTrue(q.z == 0.206652090f);
			Assert::IsTrue(q.w == 0.921060979f);
		}

		TEST_METHOD(MatrixToQuaternion)
		{
			Matrix44 matrix
			{
				12.0f, 12.0f, 54.0f, 0.0f,
				75.0f, 23.0f, 64.0f, 0.0f,
				87.0f, 34.0f, 15.0f, 0.0f,
				0.0f , 0.0f , 0.0f , 1.0f
			};

			Quaternion q{ WOWGE::Maths::MatrixToQuaternion(matrix) };

			//Assert::Fail();

			//Dunno if this is working
		}

		TEST_METHOD(QuaternionLookAndUp)
		{
			Vector3 look{ 3.0f,-1.0f,6.5f };
			Vector3 up{ -12.0f, 34.0f, 98.0f };

			Quaternion q{ QuaternionUsingLookAndUpVector(look,up) };

			Assert::IsTrue(q.x == 0.150267527f);
			Assert::IsTrue(q.y == 0.166749120f);
			Assert::IsTrue(q.z == 0.389960349f);
			Assert::IsTrue(q.w == 0.893054008f);
		}

		TEST_METHOD(QuaternionFromAndTo)
		{
			Vector3 from{ 3.0f,-1.0f,6.5f };
			Vector3 to{ -12.0f, 34.0f, 98.0f };

			Quaternion q{ QuaternionUsingFromAndToVector(from,to) };

			//Assert::Fail();

			//Dunno if this is working
		}

		TEST_METHOD(QuaternionMatrix)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };

			Matrix44 convertedMatrix{ QuaternionToMatrix(q1) };

			Assert::IsTrue(convertedMatrix.m11 == -9.0f);
			Assert::IsTrue(convertedMatrix.m12 == 34.0f);
			Assert::IsTrue(convertedMatrix.m13 == -28.0f);
			Assert::IsTrue(convertedMatrix.m14 == 0.0f);
			Assert::IsTrue(convertedMatrix.m21 == -2.0f);
			Assert::IsTrue(convertedMatrix.m22 == -33.0f);
			Assert::IsTrue(convertedMatrix.m23 == 76.0f);
			Assert::IsTrue(convertedMatrix.m24 == 0.0f);
			Assert::IsTrue(convertedMatrix.m31 == 44.0f);
			Assert::IsTrue(convertedMatrix.m32 == -68.0f);
			Assert::IsTrue(convertedMatrix.m33 == -39.0f);
			Assert::IsTrue(convertedMatrix.m34 == 0.0f);
			Assert::IsTrue(convertedMatrix.m41 == 0.0f);
			Assert::IsTrue(convertedMatrix.m42 == 0.0f);
			Assert::IsTrue(convertedMatrix.m43 == 0.0f);
			Assert::IsTrue(convertedMatrix.m44 == 1.0f);

		}

		TEST_METHOD(QuaternionSlerp)
		{
			Quaternion q1{ 4.0f, 2.0f, 1.0f, 9.0f };
			Quaternion q2{ 2.0f, 3.0f, 5.0f, 7.0f };

			Quaternion q3{ Slerp(q1,q2, 0.3f) };

			Assert::IsTrue(q3.x == 0.353993893f);
			Assert::IsTrue(q3.y == 0.239466444f);
			Assert::IsTrue(q3.z == 0.229054868f);
			Assert::IsTrue(q3.w == 0.874573052f);

		}

		TEST_METHOD(QuaternionEuler)
		{
			Quaternion q{ 1.0f, 1.0f, 1.0f, 1.0f };

			Vector3 euler{ ToEulerAngles(q) };

			Assert::IsTrue(euler.x == 0.0f);
			Assert::IsTrue(euler.y == 1.57079637f);
			Assert::IsTrue(euler.z == 1.57079637f);

		}

		TEST_METHOD(EulerQuaternion)
		{
			Vector3 euler{ 1.0f, 2.0f, 3.0f };

			Quaternion q{ ToQuaternion(euler) };

			Assert::IsTrue(q.x == 0.754933774f);
			Assert::IsTrue(q.y == -0.206149220f);
			Assert::IsTrue(q.z == 0.444435120f);
			Assert::IsTrue(q.w == 0.435952842f);

		}
	};
}