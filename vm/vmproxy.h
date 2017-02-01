#ifndef VMPROXY_H
#define VMPROXY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include "source/env.h"
#include "source/memory.h"
#include "source/names.h"
#include "plugin.h"
#define MAXSIZE 500000

// linking to the execute method of the VM
boolean vm_execute(object aProcess, int maxsteps);
// initialize new process by create new stack frame
object create_process(const char* code);
// add a job to the scheduler, multi process execution
void schedulerRun (char* text);
// execute directly a code
object goDoIt(const char* code);
// create a temporal string
void create_tmp_str(const char* code);
// load string
char* load_string(object objptr);
// execute code and return string of it
object request_dictionary(dictionary rq);
#endif // !1
