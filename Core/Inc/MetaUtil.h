#ifndef INCLUDED_CORE_METAUTIL_H
#define INCLUDED_CORE_METAUTIL_H

namespace WOWGE
{
namespace Core
{
namespace Meta
{

class MetaType;

template<class T>
inline const MetaType* GetMetaType()
{
	static_assert(sizeof(T) == -1, "Cannot deduce type");
}

template<class ClassType, class T>
inline const MetaType* GetMetaType(T ClassType::*)
{
	return GetMetaType<T>();
}

template<class ClassType, class T>
inline size_t GetFieldOffset(T ClassType::* field)
{
	return (size_t)(void*)&(((ClassType*)nullptr)->*field);
}

template<class BaseType, class DerivedType>
inline DerivedType* DynamicCast(BaseType* ptr)
{
	if (ptr->GetMetaClass() != DerivedType::StaticGetMetaClass())
	{
		return nullptr;
	}
	return static_cast<DerivedType*>(ptr);
}

template<class BaseType, class DerivedType>
inline bool IsBaseOf(BaseType* base, DerivedType* derived)
{
	//auto parent{ DerivedParent::StaticGetMetaClass()->GetParent() };
	//while (parent != nullptr)
	//{
	//	if (parent == BaseType::StaticGetMetaClass())
	//	{
	//		return true;
	//	}
	//	parent = parent->GetParent();
	//}
	return false;
}

} //namespace Meta
} //namespace Core
} //namespace WOWGE

#endif // !INCLUDED_CORE_METAUTIL_H
