#include "stdafx.h"
#include "CppUnitTest.h"

#include <Core/Inc/Core.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WOWGE::Core;

namespace BlockAllocatorTest
{		
	TEST_CLASS(BlockAllocatorTest)
	{
	public:
		
		TEST_METHOD(BlockAllocatorAllocate)
		{
			BlockAllocator blockAllocator{ 4, 2 };
			
			uint32_t* data{ nullptr };
			data = (uint32_t*)blockAllocator.Allocate();
			*data = 10;

			Assert::IsNotNull(data);
		}

		TEST_METHOD(BlockAllocatorAllocateTwo)
		{
			BlockAllocator blockAllocator{ 4, 2 };

			uint32_t* data1{ nullptr };
			uint32_t* data2{ nullptr };

			data1 = (uint32_t*)blockAllocator.Allocate();
			data2 = (uint32_t*)blockAllocator.Allocate();

			*data1 = 10;
			*data2 = 20;

			Assert::IsNotNull(data1);
			Assert::IsNotNull(data2);
		}

		TEST_METHOD(BlockAllocatorAllocateThree)
		{
			BlockAllocator blockAllocator{ 4, 2 };

			uint32_t* data1{ nullptr };
			uint32_t* data2{ nullptr };
			uint32_t* data3{ nullptr };

			data1 = (uint32_t*)blockAllocator.Allocate();
			data2 = (uint32_t*)blockAllocator.Allocate();
			data3 = (uint32_t*)blockAllocator.Allocate();

			*data1 = 10;
			*data2 = 20;

			Assert::IsNotNull(data1);
			Assert::IsNotNull(data2);

			Assert::IsNull(data3);
		}

		TEST_METHOD(BlockAllocatorFree)
		{
			BlockAllocator blockAllocator{ 4, 5 };

			uint32_t* data1{ nullptr };
			uint32_t* data2{ nullptr };
			uint32_t* data3{ nullptr };
			uint32_t* data4{ nullptr };
			uint32_t* data5{ nullptr };

			data1 = reinterpret_cast<uint32_t*>(blockAllocator.Allocate());
			data2 = reinterpret_cast<uint32_t*>(blockAllocator.Allocate());
			data3 = reinterpret_cast<uint32_t*>(blockAllocator.Allocate());
			data4 = reinterpret_cast<uint32_t*>(blockAllocator.Allocate());
			data5 = reinterpret_cast<uint32_t*>(blockAllocator.Allocate());

			*data1 = 10;
			*data2 = 20;
			*data3 = 30;
			*data4 = 40;
			*data5 = 50;

			blockAllocator.Free(data1);
			blockAllocator.Free(data3);

			data1 = reinterpret_cast<uint32_t*>(blockAllocator.Allocate());
			*data1 = 60;

			data3 = reinterpret_cast<uint32_t*>(blockAllocator.Allocate());
			*data3 = 70;

			Assert::IsTrue(*data1 == 60);
			Assert::IsTrue(*data2 == 20);
			Assert::IsTrue(*data3 == 70);
			Assert::IsTrue(*data4 == 40);
			Assert::IsTrue(*data5 == 50);

		}
	};
}