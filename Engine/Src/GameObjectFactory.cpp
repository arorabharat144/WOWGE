#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "XMLReader.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

GameObjectFactory::GameObjectFactory(GameOjectAllocator& allocator) :
	mGameObjectAllocator{ allocator }
{

}

GameObject* GameObjectFactory::Create(const char* templateFileName)
{

	rapidxml::file<> xmlFile{ templateFileName };
	rapidxml::xml_document<> xmlDocument;
	xmlDocument.parse<0>(xmlFile.data());

	//Root(GameObject)
	rapidxml::xml_node<>* xmlNode{ xmlDocument.first_node() };

	GameObject* gameObject{ nullptr };
	
	if (strcmp(xmlNode->name(), "GameObject") == 0)
	{
		gameObject = mGameObjectAllocator.New();
		if (strcmp(xmlNode->first_node()->name(), "Components") == 0)
		{
			rapidxml::xml_node<>* componentNode{ xmlNode->first_node()->first_node() };
			//Iterate through components
			while (componentNode != nullptr)
			{
				std::string componentName{ componentNode->name() };
				const Core::Meta::MetaClass* metaClass{ Core::Meta::MetaRegistry::FindMetaClass(componentName.c_str()) };
				if (metaClass != nullptr)
				{
					Component* newComponent{ gameObject->AddComponent(metaClass) };
					const rapidxml::xml_node<>* data{ componentNode->first_node() };
					VERIFY(XMLReader::Read(newComponent, metaClass, data), "Failed to create %s", metaClass->GetName());
				}

				componentNode = componentNode->next_sibling();
			}
		}
	}

	return gameObject;
}
void GameObjectFactory::Destroy(GameObject* gameObject)
{
	mGameObjectAllocator.Delete(gameObject);
}