#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include "plugin.h"

#include "source/env.h"
#include "source/memory.h"
#include "source/names.h"

pthread_mutex_t exec_mux;

int initial = 0;		/* not making initial image */
void init();
call __init__ = init;
boolean vm_execute(object aProcess, int maxsteps);

object create_process(const char* code)
{
	//printf("%s\n", code);
    object process, stack, method, processClass;
    method = newMethod();
    setInstanceVariables(nilobj);
    ignore parse(method, code, false);
	/*must check if the parse method success or not to
	continue the execution, this will avoid the segment
	fault error*/
    process = allocObject(processSize);
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
object schedulerRun (char* text)
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
	object process = create_process(code);
	while (vm_execute(process, 5000));
	decr(process);
	return basicAt(basicAt(process, stackInProcess),1);
}
void init()
{
	FILE *fp; 
	char* path = __s("%s/%s",__plugin__.pdir,"image.im");
    fp = fopen(path,"r");
    
	if (fp == NULL) {
		printf("Cannot load image : %s\n", path);
		return;
    }
	pthread_mutex_init(&exec_mux, NULL);
    initMemoryManager();
    imageRead(fp);
    initCommonSymbols();
    //dumpInitialiseImage();
    //stdin <- File new; name: 'stdin'; mode: 'r'; open. \
    // create process and execute it
	goDoIt("webGlobal \
			stderr <- File new; name: 'stdout'; mode: 'w'; open. \
			stdout <- File new; name: 'stdout'; mode: 'w'; open. \
        	sysTmp <- ''. \
			scheduler <- Scheduler new.\
    		imgMeta <- ImageManager new.\
			webProcess <- [scheduler runOne] newProcess.");
    LOG("%s","Finish load image\n");
}
void pexit()
{
	pthread_mutex_destroy(&exec_mux);
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
						R_STR(rq,"class"));
		methods = result_string_of(code);
		
		code = __s("x ^(imgMeta variablesOf:%s)",
				R_STR(rq,"class"));
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
		char* code = R_STR(rq,"code");
        create_tmp_str(code);
        // create the method
        code = __s("x ^(imgMeta addMethodTo:%s)",R_STR(rq,"class"));
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
		char* name = R_STR(rq,"name");
		if(!name) name = "backup";
		//printf("%s\n", );
		char* code = __s("x smalltalk saveImage:'%s/../plugins/%s'",__plugin__.htdocs, name);
		ignore goDoIt(code);
		__t(client,"{\"result\":1}");
		return;
	}
	__t(client,"{}");
}

void update_method(int client,const char* method,dictionary rq)
{ 
	json(client);

	if(IS_POST(method))
	{
        char* code = R_STR(rq,"code");
        create_tmp_str(code);
        // now execute the code
		code = __s("x ^(imgMeta editMethod:#%s of:%s)",
                         R_STR(rq,"method"),R_STR(rq,"class"));
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
					R_STR(rq,"class"));
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
				R_STR(rq,"class"));
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
		char* code = __s("x ^(%s)",R_STR(rq,"code"));
		char* class = R_STR(rq,"class");
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
		char* code = __s("x ^(imgMeta sourceOf:'%s' in:%s)",R_STR(rq,"method"),R_STR(rq,"class"));
		__t(client,result_string_of(code));
		return;
	}
	__t(client,"");
}
void exp_class(int client, const char* method, dictionary rq)
{
	if(IS_GET(method))
	{
		char* name = R_STR(rq, "name");
		if(name)
		{
			char * file = __s("%s/../tmp/%s.st", __plugin__.htdocs, name);
			char * code = __s("x ^(imgMeta exportClass:%s to:'%s')",
							name,file);
			ignore goDoIt(code);
			octstream(client,__s("%s.st",name));
			__f(client, file);
			return;
		}
		html(client);
		__t(client,"Unknow class name");
		return;
	}
	html(client);
	__t(client,"Bad request");
}
void get_image(int client, const char* method, dictionary rq)
{
	char* name = __s("backup_%ul", time(NULL));
	char * file = __s("%s/../tmp/%s", __plugin__.htdocs, name);
	char* code = __s("x smalltalk saveImage:'%s'",file);
	ignore goDoIt(code);
	octstream(client,"FireflySTImage.im");
	__fb(client,file);
}
void kwdump(int client, const char* method, dictionary rq)
{
	char * file = __s("%s/img.kw", __plugin__.pdir);
	char* code = __s("x imgMeta imageKeywords:'%s'",file);
	ignore goDoIt(code);
	json(client);
	__t(client,"{\"result\":1,\"msg\":\"OK\"}");
}
void scriptbin(int client, const char* method, dictionary rq)
{
	FILE* fp;
	char* file = __s("%s/scriptbin.json",__plugin__.pdir);
	json(client);
	if(IS_POST(method))
	{
		char* code = __s(",{\"id\":%d,\"text\":\"%s\"}",time(NULL),R_STR(rq, "code"));
		if(code)
		{
			fp = fopen(file,"a");
			if(fp)
			{
				fwrite(code,1,strlen(code),fp);
				__t(client,"{\"result\":1,\"msg\":\"OK\"}");
				fclose(fp);
				free(code);
				return;
			}
			__t(client,"{\"result\":0,\"msg\":\"Cannot open file to write\"}");
			free(code);
			return;
		}
		__t(client,"{\"result\":0,\"msg\":\"Bad request\"}");
	}
	else
	{
		__t(client,"{\"status\": \"success\", \"items\": [");
		__fb(client,file);
		__t(client,"]}");
		free(file);
	}
}
int tty_read_buf(int fd, char*buf,int size)
{
	int i = 0;
	char c = '\0';
	int n;
	while ((i < size - 1) && (c != '\n'))
	{
		n = read(fd, &c, 1);
		if (n > 0)
		{
			buf[i] = c;
			i++;
		}
		else if(n == -1) return n;
		else
			c = '\n';
	}
	buf[i] = '\0';
	return i;
}
void webtty(int client, const char* m, dictionary rq)
{
	textstream(client);
	int filedes[2];
	char* code = R_STR(rq, "code");
	if(!code) return;
	char* query = __s("x %s",code);
	if(pipe(filedes) == -1)
	{
		perror("pipe");
		return;
	}
	pid_t pid = fork();
	if(pid == -1)
	{
		perror("folk");
		return;
	} else if(pid == 0)
	{
	    while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
	     close(filedes[1]);
	     close(filedes[0]);
	    // execute Smalltalk code, and redirect ouput to socket
		 pthread_mutex_lock (&exec_mux);
		 ignore schedulerRun(query);
		 pthread_mutex_unlock (&exec_mux);
	    // perror("execl");
	     _exit(1);
	}
	close(filedes[1]);
	char buffer[1024];
	while (1) {
		ssize_t count = tty_read_buf(filedes[0],buffer, sizeof(buffer));
		if (count == -1) {
			if (errno == EINTR) {
				continue;
			} else {
				perror("read");
				return;
			}
		} else if (count == 0) {
			break;
		} else {
			__t(client,"data:%s\n",buffer);
			//handle_child_process_output(buffer, count);
		}
	}
	close(filedes[0]);
	wait(0);
	free(code);
	free(query);
	printf("Child process exit\n");
}