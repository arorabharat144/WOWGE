#ifndef INCLUDED_CORE_HANDLE_H
#define INCLUDED_CORE_HANDLE_H

namespace WOWGE
{
namespace Core
{

template<class T>
class HandlePool;

template<class T>
class Handle
{
public:
	Handle();

	bool IsValid();
	void Invalidate();

	T* Get() const;
	T* operator->() const;

	bool operator==(Handle rhs) const { return mIndex == rhs.mIndex && mGeneration == rhs.mGeneration; }
	bool operator!=(Handle rhs) const { return !(*this == rhs); }
private:
	friend class HandlePool<T>;

	static HandlePool<T>* sPool;

	uint32_t mIndex : 24;     //Bit packing, specify the actual bits used in memory
	uint32_t mGeneration : 8; //But using it is like using a normal uint32_t
};

template<class T>
HandlePool<T>* Handle<T>::sPool = nullptr;

template<class T>
Handle<T>::Handle() :
	mIndex{ 0 },
	mGeneration{ 0 }
{

}

template<class T>
bool Handle<T>::IsValid()
{
	return sPool && sPool->IsValid(*this);
}

template<class T>
void Handle<T>::Invalidate()
{
	*this = Handle();
}

template<class T>
T* Handle<T>::Get() const
{
	return sPool ? sPool->Get(*this) : nullptr;
}

template<class T>
T* Handle<T>::operator->() const
{
	return sPool ? sPool->Get(*this) : nullptr;
}

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_CORE_HANDLE_H
