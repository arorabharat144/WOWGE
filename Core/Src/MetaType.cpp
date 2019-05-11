#include "Precompiled.h"
#include "MetaType.h"

using namespace WOWGE;
using namespace WOWGE::Core;
using namespace WOWGE::Core::Meta;

MetaType::MetaType(const char* const name, Category category, size_t size, CreateFunc createFunc, DestroyFunc destroyFunc) :
	mName{ name },
	mCategory{ category },
	mID{ std::hash_value(name) }, //std::hash<std::string>()(name)
	mSize{ size },
	mCreate{ std::move(createFunc) },
	mDestroy{ std::move(destroyFunc) }
{

}