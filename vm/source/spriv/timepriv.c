
#include "timepriv.h"
extern object processStack;
extern int linkPointer;
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

object priv_process_from_block(object block)
{
    object process = allocObject(3);
    object stack = allocObject(50);
	setClass(stack, globalSymbol("Array"));
	// find the sheduler
    setClass(process,globalSymbol("Process"));
    basicAtPut(process, 1, stack);
    basicAtPut(process, 2, newInteger(10));
    basicAtPut(process, 3, newInteger(2));

    /* put argument on stack */
    basicAtPut(stack, 1, nilobj);				/* argument */
    /* now make a linkage area in stack */
    basicAtPut(stack, 2, nilobj);				/* previous link */
    basicAtPut(stack, 3, basicAt(block, 1));	/* context object (nil = stack) */
    basicAtPut(stack, 4, newInteger(1));		/* return point */
    basicAtPut(stack, 5, nilobj);				/* method */
    basicAtPut(stack, 6, basicAt(block,4));		/* byte offset */
	return process;
}

object priv_time_to_run(object block)
{
	object savedStack = processStack;
	int savedLp = linkPointer;
	clock_t tick1, tick2;
	int tick;
	float duration_s;
    object process = priv_process_from_block(block);
    tick1 = clock();
   	while(vm_execute(process, 5000));
    tick2 = clock();
    tick = tick2-tick1;
    duration_s = ((double)tick)/((float)CLOCKS_PER_SEC);
	processStack = savedStack;
	linkPointer = savedLp;
	return newFloat((double)duration_s);
}
/*
void context_dump(object context)
{
	printf("Link location %d\n",basicAt(context,1)>>1 );
	printf("Method %d\n",basicAt(context,2)>>1 );
	int size = sizeField(basicAt(context,3));
	printf("Arguments number %d\n",size);
	for(size_t i = 0; i < size; ++i)
	{
		printf("\t %d\n",*(sysMemPtr(basicAt(context,3))+i));
	}
	
	size = sizeField(basicAt(context,4));
	printf("Temps number %d\n",size);
	for(size_t i = 0; i < size; ++i)
	{
		printf("\t %d\n",*(sysMemPtr(basicAt(context,4))+i));
	}
}*/
object priv_loop_to(int from, int to, object block)
{
	object savedStack = processStack;
	int savedLp = linkPointer;
	object process = priv_process_from_block(block);
	object stack = basicAt(process,1);
	object tmps = basicAt(basicAt(block,1),4);
	for(int i = from; i<= to; i++)
	{
		basicAtPut(tmps,1,newInteger(i));
		while(vm_execute(process, 5000));
	    basicAtPut(stack, 3, basicAt(block, 1));	/* context object (nil = stack) */
	    basicAtPut(stack, 4, newInteger(1));		/* return point */
	    basicAtPut(stack, 6, basicAt(block,4));		/* byte offset */
		basicAtPut(process, 1, stack);
		incr(stack);
	}
	processStack = savedStack;
	linkPointer = savedLp;
	return trueobj;
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
			return nilobj;
		case TIME_PRIV_RT:
			return priv_time_to_run(args[1]);
		case TIME_PRIV_LOOP:
			return priv_loop_to(args[1]>>1, args[2]>>1, args[3]);
		default: return nilobj;
	}
}
