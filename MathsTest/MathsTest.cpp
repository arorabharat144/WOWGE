#include "stdafx.h"
#include "CppUnitTest.h"

#include <Math\Inc\Maths.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WOWGE::Maths;

namespace MathTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Vector3Addition)
		{
			Vector3 a{ 0.0f,1.0f,0.0f };
			Vector3 b{ 1.0f,0.0f,1.0f };
			Vector3 c = a + b;

			Assert::IsTrue(c.x == 1.0f);
			Assert::IsTrue(c.y == 1.0f);
			Assert::IsTrue(c.z == 1.0f);
		}

		TEST_METHOD(Vector3Subtraction)
		{
			Vector3 a{ 2.0f,1.0f,3.0f };
			Vector3 b{ 1.0f,1.0f,1.0f };
			Vector3 c = a - b;

			Assert::IsTrue(c.x == 1.0f);
			Assert::IsTrue(c.y == 0.0f);
			Assert::IsTrue(c.z == 2.0f);
		}

		TEST_METHOD(Vector3MultiplicationWithFloat)
		{
			Vector3 a{ 2.0f,3.0f,4.0f };
			float f = 5.0f;
			Vector3 result = a * f;

			Assert::IsTrue(result.x == 10.0f);
			Assert::IsTrue(result.y == 15.0f);
			Assert::IsTrue(result.z == 20.0f);
		}

		TEST_METHOD(Vector3DivisionWithFloat)
		{
			Vector3 a{ 50.0f,22.0f,6.0f };
			float f = 2.0f;
			Vector3 result = a / f;

			Assert::IsTrue(result.x == 25.0f);
			Assert::IsTrue(result.y == 11.0f);
			Assert::IsTrue(result.z == 3.0f);
		}

		TEST_METHOD(Vector3AdditionWithItself)
		{
			Vector3 a{ 50.0f,22.0f,6.0f };
			Vector3 b{ 2.0f,1.0f,3.0f };

			a += b;

			Assert::IsTrue(a.x == 52.0f);
			Assert::IsTrue(a.y == 23.0f);
			Assert::IsTrue(a.z == 9.0f);
		}

		TEST_METHOD(Vector3SubtractionWithItself)
		{
			Vector3 a{ 50.0f,22.0f,6.0f };
			Vector3 b{ 2.0f,1.0f,3.0f };

			a -= b;

			Assert::IsTrue(a.x == 48.0f);
			Assert::IsTrue(a.y == 21.0f);
			Assert::IsTrue(a.z == 3.0f);
		}

		TEST_METHOD(Vector3MultiplicationWithItself)
		{
			Vector3 a{ 50.0f,22.0f,6.0f };
			float num = 2.0f;

			a *= num;

			Assert::IsTrue(a.x == 100.0f);
			Assert::IsTrue(a.y == 44.0f);
			Assert::IsTrue(a.z == 12.0f);
		}

		TEST_METHOD(Vector3DivisionWithItself)
		{
			Vector3 a{ 50.0f,22.0f,6.0f };
			float num = 2.0f;

			a /= num;

			Assert::IsTrue(a.x == 25.0f);
			Assert::IsTrue(a.y == 11.0f);
			Assert::IsTrue(a.z == 3.0f);
		}

		TEST_METHOD(Vector3Dot)
		{
			Vector3 a{ 2.0f,5.0f,7.0f };
			Vector3 b{ 3.0f,6.0f,8.0f };
			float c = Dot(a, b);

			Assert::IsTrue(c == 92.0f);
		}

		TEST_METHOD(Vector3Cross)
		{
			Vector3 a{ 2.0f,5.0f,7.0f };
			Vector3 b{ 3.0f,6.0f,8.0f };
			Vector3 c = Cross(a, b);

			Assert::IsTrue(c.x == -2.0f);
			Assert::IsTrue(c.y == 5.0f);
			Assert::IsTrue(c.z == -3.0f);
		}

		TEST_METHOD(Vector3MagnitudeSquare)
		{
			Vector3 a{ 2.0f,5.0f,7.0f };
			float result = MagnitudeSquare(a);

			Assert::IsTrue(result == 78.0f);
		}

		TEST_METHOD(Vector3Magnitude)
		{
			Vector3 a{ 2.0f,5.0f,7.0f };
			float result = Magnitude(a);

			Assert::IsTrue(result == 8.83176041f);
		}

		TEST_METHOD(Vector3Normalize)
		{
			Vector3 a{ 2.0f,5.0f,7.0f };
			Vector3 resultVector = Normalize(a);

			Assert::IsTrue(resultVector.x == 0.226455420f);
			Assert::IsTrue(resultVector.y == 0.566138566f);
			Assert::IsTrue(resultVector.z == 0.792593956f);
		}

		TEST_METHOD(Vector3DistanceSquare)
		{
			Vector3 a{ 2.0f,5.0f,7.0f };
			Vector3 b{ 24.0f,56.0f,78.0f };

			float result = DistanceSquare(a, b);

			Assert::IsTrue(result == 8126.0f);
		}

		TEST_METHOD(Vector3Distance)
		{
			Vector3 a{ 2.0f,5.0f,7.0f };
			Vector3 b{ 24.0f,56.0f,78.0f };

			float result = Distance(a, b);

			Assert::IsTrue(result == 90.144329f);
		}

		TEST_METHOD(Vector3Project)
		{
			Vector3 a{ -1.0f,4.0f,2.0f };
			Vector3 b{ 1.0f,0.0f,3.0f };

			Vector3 c = Project(a, b);

			Assert::IsTrue(c.x == 0.5f);
			Assert::IsTrue(c.y == 0.0f);
			Assert::IsTrue(c.z == 1.5f);
		}

		TEST_METHOD(MatrixMultiplication)
		{
			Matrix44 a
			{
				1.0f, 4.0f, 3.0f, 0.0f,
				5.0f, 1.0f, 2.0f, 0.0f,
				6.0f, 7.0f, 1.0f, 0.0f,
				1.0f, 2.0f, 3.0f, 4.0f
			};

			Matrix44 b
			{
				1.0f, 7.0f, 3.0f, 0.0f,
				1.0f, 1.0f, 2.0f, 0.0f,
				5.0f, 6.0f, 1.0f, 0.0f,
				1.0f, 2.0f, 3.0f, 4.0f
			};

			Matrix44 c
			{
				20.0f, 29.0f, 14.0f, 0.0f,
				16.0f, 48.0f, 19.0f, 0.0f,
				18.0f, 55.0f, 33.0f, 0.0f,
				22.0f, 35.0f, 22.0f, 16.0f
			};

			Assert::IsTrue(a * b == c);

		}

	};
}