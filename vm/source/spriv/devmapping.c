#include "devmapping.h"
//static unsigned char* sysdevs[MAX_DEV] ={NULL};

object dev_open(const char* path, int size)
{
	int devf = open(path, O_RDWR|O_SYNC);
	unsigned char* ptr;
	if(devf <0)
	{
    	printf("Cannot open file %s\n", path);
    	return nilobj;
	}
	ptr = mmap(0,size,PROT_READ|PROT_WRITE, MAP_SHARED, devf, 0);
	if(ptr == MAP_FAILED)
	{
		ptr = NULL;
    	printf("MMap faile %s\n",path);
    	return nilobj;
	}
	// close the file
	if(close(devf) == -1)
	{
		printf("Cant close the file: %s\n",path);
		return nilobj;
	}
	//sysdevs[idx] = ptr;
	//printf("Map ok\n");
	return newPointer(ptr);
}
object dev_map(object idx,int size)
{
	unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	if(ptr)
	{
		object re = allocObject(0);
		sizeField(re) = -size;
		
		objectTable[re>>1].memory = (object*)ptr;
		setClass(re,globalSymbol("ByteArray"));
		
		//printf("Memory map ok %d\n", classField(re));
		return re;
	}
	return nilobj;
}
void dev_close(object idx, object ref)
{
	int size = objectTable[ref>>1].size;
	objectTable[ref>>1].memory = NULL;
	objectTable[ref>>1].size = 0;
	unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	if(ptr)
	{
		munmap(ptr, size);
		//sysdevs[idx] = NULL;
	}
}
object  dev_read_data16(object idx, int offset)
{
	unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	if(ptr)
		return newInteger(*((short*)(ptr+offset)));
	return nilobj;
}
object  dev_read_data32(object idx, int offset)
{
	unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	if(ptr)
	{
		return newInteger(*((int*)(ptr+offset)));
	}
	return nilobj;
}
object  dev_write_data16(object idx, int offset, int data)
{
	unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	if(ptr)
	{
		*((short*)(ptr+offset)) = (short)data;
		return trueobj;
	}
	return falseobj;
}
object  dev_write_data32(object idx, int offset, int data)
{
	unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	if(ptr)
	{
		*((int*)(ptr+offset)) = (int)data;
		return trueobj;
	}
	return falseobj;
}
object dev_priv(object* args)//3 arguments
{
	int type = args[0]>>1;
	int idx, size;
	char* path;
	object data;
	switch(type)
	{
		case DEV_O: //open device
			path = charPtr(args[1]);
			size = args[2]>>1;
			return dev_open(path,size);
			
		case DEV_C: // close device
			idx = args[1];
			data = args[2];
			dev_close(idx,data);
			return nilobj;
			
		case DEV_M: // map device to ByteArray object
			idx = args[1];
			size = args[2]>>1;
			//printf("Get the bytearray\n");
			return dev_map(idx, size);
			
		case DEV_R16:// index begin at 1
			 idx = args[1];
			 size = (args[2]>>1) - 1;// this is the offset address
			 return dev_read_data16(idx, size);
			 
 		case DEV_R32:// index begin at 1
 			 idx = args[1];
 			 size = (args[2]>>1) - 1;// this is the offset address
 			 return dev_read_data32(idx, size);
			 
		case DEV_W16:// index begin at 1
			return dev_write_data16(args[1], (args[2]>>1)-1, args[3] >> 1);
			
		case DEV_W32:// index begin at 1
			return dev_write_data32(args[1], (args[2]>>1)-1, args[3] >> 1);
		default: 
			printf("Unsupported device primitive:%d\n",type );
			return nilobj;
			break;
	}
}
