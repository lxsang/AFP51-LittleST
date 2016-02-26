#include "spriv.h"

object priv(int number, object* parameter)
{
	int high_priv = number - 150;
	switch(high_priv)
	{
		case 1:
			//pointer primitive
			return pointer_priv(parameter);
		case 2:
			// primitive 151 device mapping
			return dev_priv(parameter);
			
		case 3:
			// time primitive
			return time_priv(parameter);
#ifdef ROS_INC
		case 4: // ROS primitive
			return rospriv(parameter);
#endif
		default: 
			sysError("Unknown primitive", "sysPrimitive");
			return nilobj;
	}
}

