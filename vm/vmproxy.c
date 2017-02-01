#include "vmproxy.h"
int initial = 0;		/* not making initial image */

object create_process(const char* code)
{
	//printf("%s\n", code);
    object process, stack, method, processClass;
    method = newMethod();
    setInstanceVariables(nilobj);
	if (parse(method, code, false) == false)
	{
		parse(method,
			"x ^ (smalltalk error:'compiler error: syntax error. Please check')",
			false);
	}
	/*must check if the parse method success or not to
	continue the execution, this will avoid the segment
	fault error*/
    process = allocObject(processSize);
	incr(process);
    stack = allocObject(50);
	setClass(stack, globalSymbol("Array"));
	// find the sheduler
    processClass = globalSymbol("Process");
    setClass(process,processClass);
    basicAtPut(process, stackInProcess, stack);
    basicAtPut(process, stackTopInProcess, newInteger(10));
    basicAtPut(process, linkPtrInProcess, newInteger(2));

    /* put argument on stack */
    basicAtPut(stack, 1, nilobj);	/* argument */
    /* now make a linkage area in stack */
    basicAtPut(stack, 2, nilobj);	/* previous link */
    basicAtPut(stack, 3, nilobj);	/* context object (nil = stack) */
    basicAtPut(stack, 4, newInteger(1));	/* return point */
    basicAtPut(stack, 5, method);	/* method */
    basicAtPut(stack, 6, newInteger(1));	/* byte offset */
	return process;
}
void schedulerRun (char* text)
{
    object process, scheduler;
	process = create_process(text);
	scheduler = globalSymbol("scheduler");
    if(scheduler != nilobj)
	{
		// create new Link Object
		object lk = newLink(nilobj,process);
		// get the set object from scheduler
		object set = basicAt(scheduler,2);
		object lks = basicAt(set,1);
		// insert the process to the processList
		basicAtPut(lk, 3, lks);
		basicAtPut(set,1, lk);
		basicAtPut(scheduler,3,process);
	}
	object wprocess = globalSymbol("webProcess");
	//object stack = basicAt(wprocess, stackInProcess);
    /* now go execute it */
	// run the scheduler, not the process which will run all the
	// process in its processlist
    while (vm_execute(wprocess, 5000));
	//fprintf(stderr, "..");
}
object goDoIt(const char* code)
{
	printf("Query %s \n",code);
	object process = create_process(code);
	while (vm_execute(process, 5000));
	//decr(process);
	return process;
}

void create_tmp_str(const char* code)
{
        object str;
        
        str = newStString(code);
        decr(globalSymbol("sysTmp"));
        //assign symbol to value
        nameTableInsert(symbols, strHash("sysTmp"),
                        globalKey("sysTmp"), str);
        // printf("result %s\n", load_string(str));
}
char* load_string(object objptr);
/*{
	char* data;
	struct objectStruct objs;
	objs = objectTable[objptr>>1];
	if(objs.size < 0)
	{
		char tmp[-objs.size];
		for(int j= 0;j<-objs.size;j++)
		{
			tmp[j] = byteAt(objptr,j+1);
		}
		data = __s("%s",tmp);
		return data;
	} else {
		//printf("class %d\n", objs.size );
		return "Error when loading resource";
	}
}*/
char* result_string_of(const char* code)
{	
	//printf("Query %s\n", code);
	char * data;
	object process = goDoIt(code);
	object result = basicAt(basicAt(process, stackInProcess),1);
	if(result == nilobj) 
		data = "{}";
	else
	{
		data = strdup(load_string(result));
	}
	decr(process);
	printf("object count %d\n", objectCount());
	return data;
}
object request_dictionary(dictionary rq)
{
	object dict = newDictionary(30);
	association asc;
	if(rq)
	{
		for_each_assoc(asc,rq)
		{
			printf("%s->%s", asc->key, asc->value);
			object st = newStString((char*) asc->value);
			nameTableInsert(dict,strHash(asc->key), newSymbol(asc->key),st);
		}
	}
	return dict;
}