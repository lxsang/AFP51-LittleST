#include "pointerpriv.h"

object newPointer(void* data)
{
	object newObj = allocObject(0);
	setClass(newObj, globalSymbol(PT_CLNAME));
	objectTable[newObj>>1].memory = (object*) data;
	return newObj;
}
object pointer_priv(object* args)
{
	int type = args[0]>>1;
	switch(type)
	{
		case PT_PRIV_STR:
			return pointer_priv_as_string(args[1]);
		case PT_PRIV_INT:
			return pointer_priv_as_integer(args[1]);
		case PT_PRIV_BA:
			return pointer_priv_as_bytearray(args[1]);
		default: return nilobj;
	}
}
object pointer_priv_as_string(object pt)
{
	return newStString((char*)(*(char**)sysMemPtr(pt)));
}
object pointer_priv_as_integer(object pt)
{
	return newInteger(*(sysMemPtr(pt)));
}
object pointer_priv_as_bytearray(object pt)
{
	//this is not implemented yet
	return nilobj;
}