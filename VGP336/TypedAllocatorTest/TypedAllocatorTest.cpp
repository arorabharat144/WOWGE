#include "stdafx.h"
#include "CppUnitTest.h"

#include <Core/Inc/Core.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace WOWGE::Core;

namespace TypedAllocatorTest
{		
	class Dog
	{
	public:
		Dog(float pos = 0.0f) :
			position{ pos } {}

		float position;
		std::string name;
		std::string breed;
	};

	TEST_CLASS(TypedAllocatorTest)
	{
	public:
		
		TEST_METHOD(TypedAllocatorNew)
		{
			TypedAllocator<Dog> allocator{ 3 };
			Dog* dogs[3];
			for (uint32_t i{ 0 }; i < 3; ++i)
			{
				dogs[i] = allocator.New(10.0f);
			}
			dogs[0]->name = "Brayden";
			dogs[0]->breed = "Dachshund";

			dogs[1]->name = "Jimmy";
			dogs[1]->breed = "Labrador Retriever";

			dogs[2]->name = "Timmy";
			dogs[2]->breed = "German Shephard";

			Assert::IsTrue(dogs[0]->name.compare("Brayden") == 0);
			Assert::IsTrue(dogs[0]->breed.compare("Dachshund") == 0);

			Assert::IsTrue(dogs[1]->name.compare("Jimmy") == 0);
			Assert::IsTrue(dogs[1]->breed.compare("Labrador Retriever") == 0);

			Assert::IsTrue(dogs[2]->name.compare("Timmy") == 0);
			Assert::IsTrue(dogs[2]->breed.compare("German Shephard") == 0);
		}

		TEST_METHOD(TypedAllocatorDelete)
		{
			TypedAllocator<Dog> allocator{ 3 };
			Dog* dogs[3];
			for (uint32_t i{ 0 }; i < 3; ++i)
			{
				dogs[i] = allocator.New(10.0f);
			}
			dogs[0]->name = "Brayden";
			dogs[0]->breed = "Dachshund";

			dogs[1]->name = "Jimmy";
			dogs[1]->breed = "Labrador Retriever";

			dogs[2]->name = "Timmy";
			dogs[2]->breed = "German Shephard";

			allocator.Delete(dogs[2]);

			Assert::IsTrue(dogs[2]->name.compare("Timmy") != 0);
			Assert::IsTrue(dogs[2]->breed.compare("German Shephard") != 0);
		}

		TEST_METHOD(TypedAllocatorComplete)
		{
			TypedAllocator<Dog> allocator{ 3 };
			Dog* dogs[3];
			for (uint32_t i{ 0 }; i < 3; ++i)
			{
				dogs[i] = allocator.New(10.0f);
			}
			dogs[0]->name = "Brayden";
			dogs[0]->breed = "Dachshund";

			dogs[1]->name = "Jimmy";
			dogs[1]->breed = "Labrador Retriever";

			dogs[2]->name = "Timmy";
			dogs[2]->breed = "German Shephard";

			allocator.Delete(dogs[2]);

			allocator.New(20.0f);

			dogs[2]->name = "Dog";
			dogs[2]->breed = "Bulldog";

			Assert::IsTrue(dogs[2]->name.compare("Dog") == 0);
			Assert::IsTrue(dogs[2]->breed.compare("Bulldog") == 0);
		}

	};
}