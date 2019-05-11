#ifndef INCLUDED_CORE_META_H
#define INCLUDED_CORE_META_H

#include "MetaClass.h"
#include "MetaField.h"
#include "MetaType.h"
#include "MetaUtil.h"

#define META_TYPE_DECLARE(Type)\
	template<> const WOWGE::Core::Meta::MetaType* WOWGE::Core::Meta::GetMetaType<Type>();

#define META_TYPE_DEFINE(Type, Name)\
	template<> const WOWGE::Core::Meta::MetaType* WOWGE::Core::Meta::GetMetaType<Type>()\
	{\
		static WOWGE::Core::Meta::MetaType sMetaType {\
			Name,\
			WOWGE::Core::Meta::MetaType::Category::Primitive,\
			sizeof(Type),\
			[] { return new Type; },\
			[] (void* data) { delete static_cast<Type*>(data); },\
			};\
		return &sMetaType;\
	}

#define META_CLASS_DECLARE\
	static const WOWGE::Core::Meta::MetaClass* StaticGetMetaClass();\
	virtual const WOWGE::Core::Meta::MetaClass* GetMetaClass() const{ return StaticGetMetaClass(); }

#define META_CLASS_BEGIN_INTERNAL(ClassType)\
	template<> const WOWGE::Core::Meta::MetaType* WOWGE::Core::Meta::GetMetaType<ClassType>()\
	{\
		return ClassType::StaticGetMetaClass();\
	}\
	const WOWGE::Core::Meta::MetaClass* ClassType::StaticGetMetaClass()\
	{\
		using Class = ClassType;\
		const char* const className = #ClassType;

#define META_DERIVED_BEGIN(ClassType, ParentType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const WOWGE::Core::Meta::MetaClass* parentMetaClass { ParentType::StaticGetMetaClass() };

#define META_CLASS_BEGIN(ClassType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const WOWGE::Core::Meta::MetaClass* parentMetaClass { nullptr };

#define META_FIELD_BEGIN\
		static const WOWGE::Core::Meta::MetaField fields[] = {

#define META_FIELD(Var, Name)\
			WOWGE::Core::Meta::MetaField { Name, WOWGE::Core::Meta::GetMetaType(&Class::Var), WOWGE::Core::Meta::GetFieldOffset(&Class::Var) },

#define META_FIELD_END\
		};\
		static const size_t numFields { std::size(fields) };

#define META_NO_FIELD\
		static const WOWGE::Core::Meta::MetaField* fields { nullptr };\
		static const uint32_t numFields { 0 };

#define META_CLASS_END\
		static const WOWGE::Core::Meta::MetaClass sMetaClass{ className, sizeof(Class),\
		[] { return new Class(); },\
		[] (void* data) { delete static_cast<Class*>(data); },\
		parentMetaClass, fields, numFields };\
		return &sMetaClass;\
	}

#define META_ARRAY_DECLARE(Type)\
		const WOWGE::Core::Meta::MetaArray* GetMetaArray();

#define META_ARRAY_DEFINE(Type, Name, NumberOfElements)\
		const WOWGE::Core::Meta::MetaArray* GetMetaArray()\
		{\
			static const WOWGE::Core::Meta::MetaArray sMetaArray{ #Name, sizeof(Type) * NumberOfElements,\
			[] { return new Type[NumberOfElements]; },\
			[] (void* data) { delete[] static_cast<Type*>(data); },\
		NumberOfElements\
			};\
			return &sMetaArray;\
		}

#endif // !INCLUDED_CORE_META_H
