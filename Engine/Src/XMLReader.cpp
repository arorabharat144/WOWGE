#include "Precompiled.h"
#include "XMLReader.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

namespace
{
	bool ReadInt(void* componentField, const Core::Meta::MetaField* field, const rapidxml::xml_node<>* node, size_t index)
	{
		*reinterpret_cast<int*>(reinterpret_cast<char*>(componentField) + field->GetOffset() + index * sizeof(int)) = std::stoi(node->value());
		return true;
	}

	bool ReadFloat(void* componentField, const Core::Meta::MetaField* field, const rapidxml::xml_node<>* node, size_t index)
	{
		*reinterpret_cast<float*>(reinterpret_cast<char*>(componentField) + field->GetOffset() + index * sizeof(float)) = std::stof(node->value());
		return true;
	}

	bool ReadBool(void* componentField, const Core::Meta::MetaField* field, const rapidxml::xml_node<>* node, size_t index)
	{
		*reinterpret_cast<bool*>(reinterpret_cast<char*>(componentField) + field->GetOffset() + index * sizeof(bool)) = node->value();
		return true;
	}

	bool ReadString(void* componentField, const Core::Meta::MetaField* field, const rapidxml::xml_node<>* node, size_t index)
	{
		size_t size{ strlen(node->value()) + 1 };

		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(componentField) + field->GetOffset() + index * size) = node->value();
		return true;
	}
}

bool XMLReader::Read(void* instance, const Core::Meta::MetaClass* metaClass, const rapidxml::xml_node<>* node)
{
	while (node != nullptr)
	{
		const Core::Meta::MetaField* metaField{ metaClass->FindField(node->first_attribute()->value()) };
		if (metaField != nullptr)
		{
			rapidxml::xml_node<>* childNode{ node->first_node() };

			size_t i{ 0 };
			while (childNode != nullptr)
			{
				if (strcmp(childNode->name(), "Int") == 0)
				{
					VERIFY(ReadInt(instance, metaField, childNode, i), "Failed to read int");
				}
				else if (strcmp(childNode->name(), "Float") == 0)
				{
					VERIFY(ReadFloat(instance, metaField, childNode, i), "Failed to read float");
				}
				else if(strcmp(childNode->name(), "Bool") == 0)
				{
					VERIFY(ReadBool(instance, metaField, childNode, i), "Failed to read bool");
				}
				else if (strcmp(childNode->name(), "String") == 0)
				{
					VERIFY(ReadString(instance, metaField, childNode, i), "Failed to read string");
				}

				i++;
				childNode = childNode->next_sibling();
			}

			node = node->next_sibling();
		}
	}

	return true;
}