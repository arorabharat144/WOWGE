#include "Precompiled.h"
#include "MetaArray.h"

using namespace WOWGE;
using namespace WOWGE::Core;
using namespace WOWGE::Core::Meta;

MetaArray::MetaArray(const char* const name, size_t size, CreateFunc create, DestroyFunc destroy, size_t numElements) :
	MetaType{ name, Category::Array, size, std::move(create), std::move(destroy) },
	mNumElements{ numElements }
{

}