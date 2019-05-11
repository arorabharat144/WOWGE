#ifndef INCLUDED_ENGINE_RESOURCEMANAGER_H
#define INCLUDED_ENGINE_RESOURCEMANAGER_H

namespace WOWGE
{
namespace Engine
{

class Resource;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void Initialize();
	void Terminate();

	template<class T>
	std::string AddResource(std::string path = "");

	Resource* GetResource(std::string name);

	size_t GetResourceCount() const { return mResources.size(); }
	
private:
	using Resources = std::map<std::string, std::unique_ptr<Resource>>;

	Resources mResources;
};

template<class T>
std::string ResourceManager::AddResource(std::string path)
{
	static_assert(std::is_base_of_v<Resource, T>,
		"Can only add resource of ResourceType");

	size_t nameLocation{ path.find_last_of("/\\") };
	std::string name{ path.substr(nameLocation + 1) };

	mResources.emplace(name, std::make_unique<T>());
	return name;
}

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_RESOURCEMANAGER_H