
#include "timepriv.h"

object new_date()
{
	time_t ti;
	unsigned stamp;
	struct tm * timeinfo; 
	stamp = (unsigned) time (&ti);
	timeinfo = localtime (&ti);
	// create new date object and add value to its 
	// instance variables
	object newObj = allocObject(OBJ_SIZE);
	setClass(newObj, globalSymbol("Date"));
	basicAtPut(newObj,1,newInteger(timeinfo->tm_mday));
	basicAtPut(newObj,2,newInteger(timeinfo->tm_mon+1));
	basicAtPut(newObj,3,newInteger(timeinfo->tm_year+1900));
	basicAtPut(newObj,4,newInteger(timeinfo->tm_hour));
	basicAtPut(newObj,5,newInteger(timeinfo->tm_min));
	basicAtPut(newObj,6,newInteger(timeinfo->tm_sec));
	//printf("%d\n",stamp);
	// BUG
	//TODO: integer overflow, must convert it to LongInteger
	basicAtPut(newObj,7,newInteger(stamp));
	return newObj;
}

object time_priv(object* args)
{
	int type = args[0]>>1;
	switch(type)
	{
		case TIME_PRIV_N:
			return new_date();
		case TIME_PRIV_SL:
			usleep((args[1]>>1)*1000);
			return new_date();
		default: return nilobj;
	}
}
