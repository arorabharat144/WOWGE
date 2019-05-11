#ifndef INCLUDED_ENGINE_RESOURCE_H
#define INCLUDED_ENGINE_RESOURCE_H

namespace WOWGE
{
namespace Engine
{

class Resource
{
public:
	Resource() {}
	virtual ~Resource() {}

	virtual void Initialize(std::string filePath = "") {}

	virtual void Terminate() {}

	virtual void Render() {}

private:
};

}
}
#endif // !INCLUDED_ENGINE_RESOURCE_H
