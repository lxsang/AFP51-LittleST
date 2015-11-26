#include "spriv.h"

object priv(int number, object* parameter)
{
	int high_priv = number - 150;
	switch(high_priv)
	{
		case 1:
			// primitive 151 device mapping
			return dev_priv(parameter);
			
		case 2:
			// time primitive
			return time_priv(parameter);
		default: 
			sysError("Unknown primitive", "sysPrimitive");
	}
}