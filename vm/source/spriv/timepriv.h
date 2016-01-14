#ifndef TIMEPRIV_H
#define TIMEPRIV_H 

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "../env.h"
#include "../memory.h"
#include "../names.h"

#define TIME_PRIV_N 	0
#define TIME_PRIV_SL 	1
#define TIME_PRIV_RT	2 // time to run
#define TIME_PRIV_LOOP	3
#define OBJ_SIZE 7

object new_date();
object time_priv(object*);
object priv_time_to_run(object);
object priv_loop_to(int, int, object);
#endif