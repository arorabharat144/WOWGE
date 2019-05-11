#ifndef INCLUDED_CORE_METATYPE_H
#define INCLUDED_CORE_METATYPE_H

namespace WOWGE
{
namespace Core
{
namespace Meta
{

class MetaType
{
public:
	using CreateFunc = std::function<void*()>;
	using DestroyFunc = std::function<void(void*)>;

	enum class Category
	{
		Primitive,
		Class,
		Array,
		Pointer
	};

	MetaType(const char* const name, Category category, size_t size, CreateFunc createFunc, DestroyFunc destroyFunc);

	const char* GetName() const { return mName; }
	Category GetCategory() const { return mCategory; }
	size_t GetID() const { return mID; }
	size_t GetSize() const { return mSize; }

	void* Create() const { return mCreate(); }
	void Destroy(void* data) const { mDestroy(data); }

private:
	const char* const mName;
	Category mCategory;
	size_t mID;
	size_t mSize;

	CreateFunc mCreate;
	DestroyFunc mDestroy;
};

} //namespace Meta
} //namespace Core
} //namespace WOWGE

#endif // !INCLUDED_CORE_METATYPE_H
