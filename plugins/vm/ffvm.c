#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../plugin.h"

#include "source/env.h"
#include "source/memory.h"
#include "source/names.h"

int initial = 0;		/* not making initial image */
void init();
call __init__ = init;
boolean vm_execute(object aProcess, int maxsteps);

object goDoIt(text)
char *text;
{
    object process, stack, method, scheduler, processClass;

    method = newMethod();
    incr(method);
    setInstanceVariables(nilobj);
    // printf("parse %s\n", text);
    ignore parse(method, text, false);

    process = allocObject(processSize);
    //incr(process);
    stack = allocObject(50);
    //dump_object_header(stack);
    //incr(stack);
    scheduler = globalSymbol("scheduler");
    processClass = globalSymbol("Process");
    setClass(process,processClass);
    if(scheduler != nilobj)
    	basicAtPut(scheduler,3,process);
    /* make a process */
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
    //printf("%d\n", getClass(process));
    /* now go execute it */
    while (vm_execute(process, 15000))
	fprintf(stderr, "..");
	return basicAt(basicAt(process, stackInProcess),1);
}

void init()
{
	FILE *fp; 
	char* path = __s("%s/%s",__plugin__.pdir,"image");
    fp = fopen(path,"r");
    
	if (fp == NULL) {
		printf("Cannot load image : %s\n", path);
		return;
    }
     initMemoryManager();
    imageRead(fp);
    initCommonSymbols();
    //dumpInitialiseImage();
    //stdin <- File new; name: 'stdin'; mode: 'r'; open. \
    // create process and execute it
    goDoIt("x true <- True new. \
		false <- False new. \
		smalltalk <- Smalltalk new. \
		files <- Array new: 15. \
		stderr <- File new; name: 'stderr'; mode: 'w'; open. \
		editor <- 'nano'. \
        sysTmp <- ''. \
		scheduler <- Scheduler new.\
		classes <- Dictionary new. \
		symbols binaryDo: [:x :y |  \
			(y class == Class) \
				ifTrue: [ classes at: x put: y ] ]. \
    	imgMeta <- ImageManager new.");
    printf("Finish load image\n");
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
char* load_string(object objptr)
{
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
}
char* result_string_of(const char* code)
{	
	printf("Query %s\n", code);
	object result = goDoIt(code);
	if(result == nilobj) 
		return "{}";
	else
		return strdup(load_string(result));
}
/**
 * execute : load default browser
 * @param client 
 * @param method 
 * @param rq     
 */
void execute(int client,const char* method,dictionary rq)
{ 
	json(client);
	__t(client,result_string_of("x ^(imgMeta allSystemClasses)"));
}

void editor_ac(int client,const char* method,dictionary rq)
{ 
	json(client);
	char* file = __s("%s/img.kw",__plugin__.pdir);
	__f(client,file);
	free(file);
}

void classinfo(int client,const char* method,dictionary rq)
{ 
	json(client);

	if(IS_POST(method))
	{
		char* methods;
		char* variables;
		
		char* code = __s("x ^(imgMeta allMethodsOf:%s)",
						dvalue(rq,"class"));
		methods = result_string_of(code);
		
		code = __s("x ^(imgMeta variablesOf:%s)",
				dvalue(rq,"class"));
		variables = result_string_of(code);
		
		__t(client,"[%s,%s]", methods,variables);
		return;
	}
	__t(client,"{}");
}

void new_method(int client,const char* method,dictionary rq)
{ 
	json(client);

	if(IS_POST(method))
	{
		char* code = dvalue(rq,"code");
        create_tmp_str(code);
        // create the method
        code = __s("x ^(imgMeta addMethodTo:%s)",dvalue(rq,"class"));
		__t(client,"%s",result_string_of(code));
		return;
	}
	__t(client,"{}");
}
void save_image(int client,const char* method,dictionary rq)
{ 
	json(client);
	if(IS_POST(method))
	{
		//printf("%s\n", );
		char* code = __s("x smalltalk saveImage:'plugins/image'");
		ignore goDoIt(code);
		__t(client,"{\"result\":1}");
		return;
	}
	__t(client,"{}");
}
void run_on_ws(int client,const char* method,dictionary rq)
{ 
	json(client);
	if(IS_POST(method))
	{
		//printf("%s\n", );
		char* code = __s("x  stdout <- File new; name: 'tmp/ffvm_log.log'; mode: 'w'; open.[%s] value print.stdout close",dvalue(rq,"code"));
		printf("Qery %s\n",code );
		ignore goDoIt(code);
		__t(client,"{\"result\":1}");
		return;
	}
	__t(client,"{}");
}
void ws_log(int client,const char* method,dictionary rq)
{ 
	html(client);
	__f(client,__s("%s/../tmp/ffvm_log.log",__plugin__.htdocs));
}

void update_method(int client,const char* method,dictionary rq)
{ 
	json(client);

	if(IS_POST(method))
	{
        char* code = dvalue(rq,"code");
        create_tmp_str(code);
        // now execute the code
		code = __s("x ^(imgMeta editMethod:#%s of:%s)",
                         dvalue(rq,"method"),dvalue(rq,"class"));
		__t(client,"%s",result_string_of(code));
		return;
	}
	__t(client,"{}");
}
void methods_of(int client,const char* method,dictionary rq)
{ 
	json(client);

	if(IS_POST(method))
	{
		char* code = __s("x ^(imgMeta allMethodsOf:%s)",
					dvalue(rq,"class"));
		__t(client,result_string_of(code));
		return;
	}
	__t(client,"{}");
}
void variables_of(int client,const char* method,dictionary rq)
{ 
	json(client);

	if(IS_POST(method))
	{
		char* code = __s("x ^(imgMeta variablesOf:%s)",
				dvalue(rq,"class"));
		__t(client,result_string_of(code));
		return;
	}
	__t(client,"{}");
}
void new_class(int client,const char* method,dictionary rq)
{ 
	json(client);
	if(IS_POST(method))
	{
		char* code = __s("x ^(%s)",dvalue(rq,"code"));
		char* class = dvalue(rq,"class");
		ignore goDoIt(code);
		object result = globalSymbol(class);
		if(result == nilobj) 
			__t(client,"{\"result\":0,\"msg\":\"Error when create Class %s\"}",class);
		else
			__t(client,"{\"result\":1,\"msg\":\"OK\"}");
		printf("Query %s\n", code);
		return;
	}
	__t(client,"{'result':-1}");
}
void source(int client,const char* method,dictionary rq)
{ 
	html(client);

	if(IS_POST(method))
	{
		char* code = __s("x ^(imgMeta sourceOf:'%s' in:%s)",dvalue(rq,"method"),dvalue(rq,"class"));
		__t(client,result_string_of(code));
		return;
	}
	__t(client,"");
}
