#ifndef INCLUDED_ENGINE_XMLREADER_H
#define INCLUDED_ENGINE_XMLREADER_H

#include <External/rapidxml-1.13/RapidXML/rapidxml.hpp>
#include <External/rapidxml-1.13/RapidXML/rapidxml_iterators.hpp>
#include <External/rapidxml-1.13/RapidXML/rapidxml_print.hpp>
#include <External/rapidxml-1.13/RapidXML/rapidxml_utils.hpp>

namespace WOWGE
{
namespace Engine
{

class XMLReader
{
public:
	static bool Read(void* instance, const Core::Meta::MetaClass* metaClass, const rapidxml::xml_node<>* node);

}; //namespace XMLReader

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_XMLREADER_H
