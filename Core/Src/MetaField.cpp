#include "Precompiled.h"
#include "MetaField.h"

using namespace WOWGE;
using namespace WOWGE::Core;
using namespace WOWGE::Core::Meta;

MetaField::MetaField(const char* const name, const MetaType* const type, size_t offset) :
	mName{ name },
	mType{ type },
	mOffset{ offset }
{

}