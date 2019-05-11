#ifndef INCLUDED_CORE_METAREGISTRY_H
#define INCLUDED_CORE_METAREGISTRY_H

namespace WOWGE
{
namespace Core
{
namespace Meta
{

class MetaClass;

namespace MetaRegistry
{

void Register(const MetaClass* metaClass);

const MetaClass* FindMetaClass(const char* className);

} //namespace MetaRegitry
} //namespace Meta
} //namespace Core
} //namespace WOWGE

#endif // !INCLUDED_CORE_METAREGISTRY_H
