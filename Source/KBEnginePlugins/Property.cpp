
#include "KBEnginePluginsPrivatePCH.h"
#include "Property.h"

Property::Property():
	name(TEXT("")),
	pUtype(NULL),
	properUtype(0),
	properFlags(0),
	aliasID(-1),
	defaultValStr(TEXT(""))
{
}

Property::~Property()
{
}
