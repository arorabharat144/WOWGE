#ifndef INCLUDED_CORE_TYPEDALLOCATOR_H
#define INCLUDED_CORE_TYPEDALLOCATOR_H

#include "BlockAllocator.h"

namespace WOWGE
{
namespace Core
{

template<class T>
class TypedAllocator : private BlockAllocator
{
public:
	TypedAllocator(uint32_t capacity);

	template<class... Args>
	T* New(Args&&... args);

	void Delete(T* ptr);
};

template<class T>
TypedAllocator<T>::TypedAllocator(uint32_t capacity) :
	BlockAllocator{ sizeof(T), capacity }
{
}

template<class T>
template<class... Args>
T* TypedAllocator<T>::New(Args&&... args)
{
	T* instance{ static_cast<T*>(Allocate()) };
	if (instance != nullptr)
	{
		new(instance) T(std::forward<Args>(args)...);
	}
	return instance;
}

template<class T>
void TypedAllocator<T>::Delete(T* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}

	ptr->~T();
	Free(ptr);
}

} //namespace Core
} //namespace WOWGE

#endif // !INCLUDED_CORE_TYPEDALLOCATOR_H
