#include "Precompiled.h"
#include "BlockAllocator.h"

#include "Debug.h"

using namespace WOWGE;
using namespace WOWGE::Core;

BlockAllocator::BlockAllocator(uint32_t blockSize, uint32_t capacity) :
	mBlockSize{ blockSize },
	mCapacity{ capacity },
	mFreeSlot{ 0 }
{
	ASSERT((blockSize != 0) && (capacity != 0), "[BlockAllocator] BlockSize cannot be zero");
	mData = static_cast<int8_t*>(malloc(blockSize * capacity));

	for (uint32_t i{ 0 }; i < capacity; ++i)
	{
		if (i + 1 != capacity)
		{
			mData[i * blockSize] = i + 1;
		}
		else
		{
			mData[i * blockSize] = -1;
		}
	}
}

BlockAllocator::~BlockAllocator()
{
	free(mData);
}

void* BlockAllocator::Allocate()
{
	if (mFreeSlot != -1)
	{
		int8_t* memoryToAllocate{ &mData[mFreeSlot * mBlockSize] };
		mFreeSlot = mData[mFreeSlot * mBlockSize];
		return memoryToAllocate;
	}
	return nullptr;
}

void BlockAllocator::Free(void* ptr)
{
	int8_t* p{ reinterpret_cast<int8_t*>(ptr) };
	ptrdiff_t distance{ p - mData };

	ASSERT(distance >= 0 && distance <= static_cast<ptrdiff_t>((mBlockSize * mCapacity) - mBlockSize), "[BlockAllocator] distance between addresses cannot be negative or more than the size of the data");

	*p = mFreeSlot;
	mFreeSlot = static_cast<uint32_t>(distance) / mBlockSize;
}