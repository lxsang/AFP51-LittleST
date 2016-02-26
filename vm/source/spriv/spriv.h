#ifndef SPRIV_H
#define SPRIV_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../env.h"
#include "../memory.h"
#include "pointerpriv.h"
#include "devmapping.h"
#include "timepriv.h"
#ifdef ROS_INC
#include "rospriv.h"
#endif
#ifdef REST_INC
#include "restpriv.h"
#endif

object priv(int, object*);

#endif