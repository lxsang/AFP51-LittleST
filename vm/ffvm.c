#include "vmproxy.h"

pthread_mutex_t exec_mux;
void init();
call __init__ = init;
static object message;
void init()
{
	FILE *fp; 
	char* path = __s("%s/%s",config_dir(),"image.im");
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
	decr(goDoIt("x nil webGlobal"));
    LOG("%s","Finish load image\n");
	free(path);
}
void pexit()
{
	pthread_mutex_destroy(&exec_mux);
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
	char* file = __s("%s/img.kw",config_dir());
	__f(client,file);
	free(file);
}

void classinfo(int client,const char* method,dictionary rq)
{ 
	json(client);
	printf("This should work righ in the box\n");
	if(IS_POST(method))
	{
		char* methods = NULL;
		char* variables = NULL;
		
		char* code = __s("x ^(imgMeta allMethodsOf:%s)",
						R_STR(rq,"class"));
		methods = result_string_of(code);
		
		code = __s("x ^(imgMeta variablesOf:%s)",
				R_STR(rq,"class"));
		variables = result_string_of(code);
		
		__t(client,"[%s,%s]", methods,variables);
		free(code);
		if(methods)
			free(methods);
		//if(variables)
		//	free(variables);
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
		free(code);
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
		char* code = __s("x smalltalk saveImage:'%s/%s'",config_dir(), name);
		decr(goDoIt(code));
		__t(client,"{\"result\":1}");
		free(code);
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
		free(code);
		return;
	}
	__t(client,"{}");
}
void methods_of(int client,const char* method, const char* path,dictionary rq)
{ 
	json(client);

	if(IS_POST(method))
	{
		char* code = __s("x ^(imgMeta allMethodsOf:%s)",
					R_STR(rq,"class"));
		__t(client,result_string_of(code));
		free(code);
		return;
	}
	__t(client,"{}");
}
void variables_of(int client,const char* method, const char* path,dictionary rq)
{ 
	json(client);

	if(IS_POST(method))
	{
		char* code = __s("x ^(imgMeta variablesOf:%s)",
				R_STR(rq,"class"));
		__t(client,result_string_of(code));
		free(code);
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
		decr(goDoIt(code));// this should be fixed
		object result = globalSymbol(class);
		if(result == nilobj) 
			__t(client,"{\"result\":0,\"msg\":\"Error when create Class %s\"}",class);
		else
			__t(client,"{\"result\":1,\"msg\":\"OK\"}");
		printf("Query %s\n", code);
		free(code);
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
		free(code);
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
			decr(goDoIt(code));
			octstream(client,__s("%s.st",name));
			__f(client, file);
			free(code);
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
	decr(goDoIt(code));
	octstream(client,"FireflySTImage.im");
	__fb(client,file);
	free(code);
}
void kwdump(int client, const char* method, dictionary rq)
{
	char * file = __s("%s/img.kw", config_dir());
	char* code = __s("x imgMeta imageKeywords:'%s'",file);
	decr(goDoIt(code));
	json(client);
	__t(client,"{\"result\":1,\"msg\":\"OK\"}");
}
void scriptbin(int client, const char* method, dictionary rq)
{
	FILE* fp;
	char* file = __s("%s/scriptbin.json",config_dir());
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
void stdout_to_socket(int client,const char* code)
{
	textstream(client);
	int filedes[2];
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
		 ignore schedulerRun(code);
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
			__t(client,"data:%s\n\n",buffer);
			//handle_child_process_output(buffer, count);
		}
	}
	close(filedes[0]);
	wait(0);
	//free(code);
}
void webtty(int client, const char* m, dictionary rq)
{
	int filedes[2];
	char* code = R_STR(rq, "code");
	if(!code) return;
	char* query = __s("x %s",code);
	stdout_to_socket(client,query);
	//free(code);
	free(query);
	printf("Child process exit\n");
}
void load_source(int c, const char* m, dictionary rq)
{
	json(c);
	char* path = NULL;
	if(IS_GET(m))
	{
		__t(c,__RESULT__,0,"Bad request:GET");
		return;
	}
	if(R_STR(rq,"st_src.file") == NULL || R_INT(rq,"st_src.size") > MAXSIZE)
	{
		__t(c,__RESULT__,0,"File not found or too large");
		return;
	}
	path = R_STR(rq,"st_src.tmp");
	char* src = __s("x <120 1 '%s' 'r'>. <123 1>. <121 1>", path);
	//printf("%s\n", src);
	decr(goDoIt(src));
	__t(c,__RESULT__,1,"OK");
	free(src);
}
void node_status(int c, const char* m, dictionary rq)
{
	json(c);
	__t(c,__RESULT__,1,"I'm online, there will be more here lately");
}
void portal(int c, const char* m, dictionary rq)
{
	
	char* code = R_STR(rq,"code");
	char* query = NULL;
	char* file = NULL;
	int streaming = R_INT(rq,"streaming");
	//printf("%s %d\n", code, streaming);
	switch(streaming)
	{
		case 0: // roundtrip request
			json(c);
			query  = __s("x ^ (imgMeta %s) asJSON",code);
			__t(c, result_string_of(query));
			//wait for all data to be received
			//decr(goDoIt("x 100 milliseconds wait"));
			break;
		case 1:// stream request
			query = __s("x (imgMeta %s)",code);
			stdout_to_socket(c,query);
			printf("Child process exit\n");
			break;
		case 2: // roundtrip request with file attached
			json(c);
			if(R_STR(rq,"xware.file") == NULL)
				__t(c,"%s","File not found.");
			else if(R_INT(rq,"xware.size") > MAXSIZE )
				__t(c,"%s","File is too large.");
			else
			{
				query  = __s("x ^ (imgMeta %s) asJSON",code);
				file = R_STR(rq,"xware.tmp");
				create_tmp_str(file);
				__t(c,"%s", result_string_of(query));
				free(file);
			}
			break;
		default: break;
	}
	if(query)
		free(query);
}

static int messTest(object obj)
{
    return obj == message;
}

/**
	Main handler method for the plugin
*/

void handler(int client, const char* method, const char* rqpth, dictionary rq)
{
	 object process, stack, processClass;
	object dict = request_dictionary(rq);
	nameTableInsert(dict,(strHash("uri")%10)*3,newSymbol("uri"),newStString(rqpth));
	printf("%d %d", strHash("uri"), (strHash("uri")%10)*3);
	object proxy = allocObject(2);
	basicAtPut(proxy,1,newInteger(client));
	basicAtPut(proxy,2,dict);
	object class = globalSymbol("VMProxy");
	setClass(proxy,class);
	message = newSymbol("handle");
	object mt = hashEachElement(basicAt(class,3),message,messTest);
	/*create new process, and execute it*/

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
    basicAtPut(stack, 1, proxy);	/* argument */
    /* now make a linkage area in stack */
    basicAtPut(stack, 2, nilobj);	/* previous link */
    basicAtPut(stack, 3, nilobj);	/* context object (nil = stack) */
    basicAtPut(stack, 4, newInteger(1));	/* return point */
    basicAtPut(stack, 5, mt);	/* method */
    basicAtPut(stack, 6, newInteger(1));	/* byte offset */

	while (vm_execute(process, 5000));
}