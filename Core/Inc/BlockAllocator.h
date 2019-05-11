#ifndef INCLUDED_CORE_BLOCKALLOCATOR_H
#define INCLUDED_CORE_BLOCKALLOCATOR_H

namespace WOWGE
{
namespace Core
{

class BlockAllocator
{
public:
	BlockAllocator(uint32_t blockSize, uint32_t capacity);
	~BlockAllocator();

	BlockAllocator(const BlockAllocator&) = delete;
	BlockAllocator& operator=(const BlockAllocator&) = delete;

	void* Allocate();
	void Free(void* ptr);

	uint32_t GetCapacity() const { return mCapacity; }

protected:
	int8_t* mData;
	uint32_t mBlockSize;
	uint32_t mCapacity;

	int mFreeSlot;
};

} //namespace Core
} //namespace WOWGE
#endif // !INCLUDED_CORE_BLOCKALLOCATOR_H
