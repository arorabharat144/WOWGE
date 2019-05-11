#include "Precompiled.h"
#include "MetaClass.h"

#include "Debug.h"

#include "MetaField.h"
#include "MetaRegistry.h"

using namespace WOWGE;
using namespace WOWGE::Core;
using namespace WOWGE::Core::Meta;

MetaClass::MetaClass(const char* const name, size_t size, CreateFunc create, DestroyFunc destroy, const MetaClass* const parent, const MetaField* const fields, size_t numFields) :
	MetaType{ name, Category::Class, size, std::move(create), std::move(destroy) },
	mParent{ parent },
	mFields{ fields },
	mNumFields{ numFields }
{
	
}

const MetaClass* MetaClass::GetParent() const
{
	return mParent;
}

const MetaField* MetaClass::FindField(const char* name) const
{
	const MetaField* field{ nullptr };

	for (uint32_t i{ 0 }; i < mNumFields; ++i)
	{
		const MetaField& f{ mFields[i] };
		if (strcmp(f.GetName(), name) == 0)
		{
			field = &f;
			break;
		}
	}

	// If the field is not found, try the parent
	if (field == nullptr && mParent != nullptr)
	{
		field = mParent->FindField(name);
	}
	return field;
}

void MetaClass::Dump() const
{
	LOG("Class [%s]", GetName());
	for (size_t i{ 0 }; i < mNumFields; ++i)
	{
		auto& field{ mFields[i] };
		LOG("has field [%s] of type [%s] at offset [%zd]", field.GetName(), field.GetMetaType()->GetName(), field.GetOffset());
	}
	if (mParent)
	{
		LOG("is derived from [%s]", mParent->GetName());
	}
}