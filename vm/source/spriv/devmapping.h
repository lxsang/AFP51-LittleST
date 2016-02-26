#ifndef DEVMAPPING_H
#define DEVMAPPING_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>

#include "../env.h"
#include "../memory.h"
#include "../names.h"
#include "pointerpriv.h"

#define FILE_OK(f) ( access( f, F_OK ) != -1 )

#define MAX_DEV 20
#define DEV_O 0
#define DEV_C 1
#define DEV_M 2
#define DEV_R16 3
#define DEV_R32 4
#define DEV_W16 5
#define DEV_W32 6
#define DEV_R_BY_W 7
#define DEV_IRQ 8
#define DEV_W_BY_W 9
#define DEV_W_CHUNK 10
#define DEV_R_CHUNK 11
#define DEV_CONF_FPGA 12

// check if an integer is 16 or 32 bits
#define IS_32(v) (v & 0xFFFF0000)
#define IS_16(v) ((v >> 16) == 0 && !IS_8(v))
#define IS_8(v) ((v>>8) == 0)
// devices
object dev_open(const char*, int);
void dev_close(object,object,int);
object dev_map(object, int);
object dev_priv(object*);
object dev_read_data16(object, int);
object dev_read_data32(object, int);
object dev_write_data16(object, int , int );
object dev_write_data32(object, int , int );
object dev_read_by_words(object, int, int,int);
object dev_wait_for_irq(object);
object dev_write_data_chunk(object,object,object);
object dev_write_data_by_words(object, int, object, object);
object dev_conf_fpga(object, object);
#endif