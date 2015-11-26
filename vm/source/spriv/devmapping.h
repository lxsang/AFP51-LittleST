#ifndef DEVMAPPING_H
#define DEVMAPPING_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "../env.h"
#include "../memory.h"
#include "../names.h"

#define MAX_DEV 20
#define DEV_O 0
#define DEV_C 1
#define DEV_M 2
#define DEV_R16 3
#define DEV_R32 4
#define DEV_W16 5
#define DEV_W32 6
// devices
object dev_open(const char*, int);
void dev_close(int,object);
object dev_map(int, int);
object dev_priv(object*);
object  dev_read_data16(int, int);
object  dev_read_data32(int, int);
object  dev_write_data16(int , int , int );
object  dev_write_data32(int , int , int );
#endif