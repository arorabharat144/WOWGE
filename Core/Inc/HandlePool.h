#ifndef INCLUDED_CORE_HANDLEPOOL_H
#define INCLUDED_CORE_HANDLEPOOL_H

#include "Handle.h"

namespace WOWGE
{
namespace Core
{
template<class T>
class HandlePool
{
public:
	using HandleType = Handle<T>;

	HandlePool(uint32_t capacity);
	~HandlePool();

	HandleType Register(T* instance);
	void Unregister(HandleType handle);

	bool IsValid(HandleType handle) const;
	T* Get(HandleType handle) const;

private:
	struct Entry
	{
		T* instance{ nullptr };
		uint32_t generation{ 0 };
	};

	std::vector<Entry> mEntries;
	std::vector<uint32_t> mFreeSlots;
};

template<class T>
HandlePool<T>::HandlePool(uint32_t capacity)
{
	ASSERT(capacity > 0, "[HandlePool] Invalid capacity!");
	mEntries.resize(capacity + 1);
	mFreeSlots.reserve(capacity);
	for (uint32_t i{ capacity }; i > 0; --i)
	{
		mFreeSlots.emplace_back(i);
	}

	// Register itself with the handle type
	ASSERT(HandlePool<T>::HandleType::sPool == nullptr,
		"[HandlePool] Cannot have more than one pool for the same type!");

	HandlePool<T>::HandleType::sPool = this;
}

template<class T>
HandlePool<T>::~HandlePool()
{
	// Check that all handles are freed
	ASSERT(mFreeSlots.size() == mFreeSlots.capacity(),
		"[HandlePool] There are still registered slots!");

	// Unregister itself with the handle type
	ASSERT(HandlePool<T>::HandleType::sPool == this, "[HandlePool] Something is wrong");
	HandlePool<T>::HandleType::sPool = nullptr;
}

template<class T>
typename HandlePool<T>::HandleType HandlePool<T>::Register(T* instance)
{
	ASSERT(instance != nullptr, "[HandlePool] Invalid instance");
	ASSERT(!mFreeSlots.empty(), "[HandlePool] No more free slots available");

	// Draw the next free slot
	uint32_t slot{ mFreeSlots.back() };
	mFreeSlots.pop_back();

	// Register the instance
	mEntries[slot].instance = instance;

	// Return a handle to this entry
	HandleType handle;
	handle.mIndex = slot;
	handle.mGeneration = mEntries[slot].generation;

	return handle;
}

template<class T>
void HandlePool<T>::Unregister(HandleType handle)
{
	if (!IsValid(handle))
	{
		return;
	}

	// Increment generation to invalidate all existing handles
	mEntries[handle.mIndex].generation++;

	// Recycle the slot
	mFreeSlots.push_back(handle.mIndex);
}

template<class T>
bool HandlePool<T>::IsValid(HandleType handle) const
{
	return handle != HandleType() && mEntries[handle.mIndex].generation == handle.mGeneration;
}

template<class T>
T* HandlePool<T>::Get(HandleType handle) const
{
	return IsValid(handle) ? mEntries[handle.mIndex].instance : nullptr;
}

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_CORE_HANDLEPOOL_H
