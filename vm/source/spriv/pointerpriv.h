#ifndef POINTERPRIV_H
#define POINTERPRIV_H
#include <stdio.h>
#include <stdlib.h>

#include "../env.h"
#include "../memory.h"
#include "../names.h"

#define PT_PRIV_STR 0
#define PT_PRIV_INT 1
#define PT_PRIV_BA 	2
#define PT_CLNAME "Pointer"
object newPointer(void*);
object pointer_priv(object*);
object pointer_priv_as_string(object);
object pointer_priv_as_integer(object);
object pointer_priv_as_bytearray(object);// and much more
#endif