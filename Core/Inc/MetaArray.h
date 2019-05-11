#ifndef INCLUDED_CORE_METAARRAY_H
#define INCLUDED_CORE_METAARRAY_H

#include "MetaType.h"

namespace WOWGE
{
namespace Core
{
namespace Meta
{

class MetaArray : public MetaType
{
public:
	MetaArray(const char* const name, size_t size, CreateFunc create, DestroyFunc destroy, size_t numElements);

	//const MetaClass* GetParent() const;
	//const MetaType* FindField(const char* name) const;

	//void Dump() const;

	size_t GetNumElements() const { return mNumElements; }

private:
	const size_t mNumElements;
};

} //namespace Meta
} //namespace Core
} //namespace WOWGE

#endif // !INCLUDED_CORE_METAARRAY_H
