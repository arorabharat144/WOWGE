#include "Precompiled.h"
#include "MetaRegistry.h"

#include "Debug.h"
#include "MetaClass.h"

using namespace WOWGE;
using namespace WOWGE::Core;
using namespace WOWGE::Core::Meta;
using namespace WOWGE::Core::Meta::MetaRegistry;

namespace
{
	using MetaClassMap = std::map<std::string, const MetaClass*>;
	inline MetaClassMap& GetRegistry()
	{
		static MetaClassMap sMetaClassMap;
		return sMetaClassMap;
	}
}

void WOWGE::Core::Meta::MetaRegistry::Register(const MetaClass* metaClass)
{
	GetRegistry().emplace(metaClass->GetName(), metaClass);
}

const MetaClass* WOWGE::Core::Meta::MetaRegistry::FindMetaClass(const char* className)
{
	MetaClassMap::const_iterator iter{ GetRegistry().find(className) };
	ASSERT(iter != GetRegistry().end(), "[MetaRegistry] Meta class for %s not found", className);
	return iter->second;
}