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
	/*if(close(devf) == -1)
	{
		printf("Cant close the file: %s\n",path);
		return nilobj;
	}*/
	object arr = newArray(2);
	basicAtPut(arr,1,newInteger(devf));
	basicAtPut(arr,2,newPointer(ptr));
	//sysdevs[idx] = ptr;
	//printf("Map ok\n");
	return  arr;
	//return newPointer(ptr);
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
void dev_close(object idx, object ref, int devf)
{
	int size = objectTable[ref>>1].size;
	objectTable[ref>>1].memory = NULL;
	objectTable[ref>>1].size = 0;
	unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	if(close(devf) == -1)
	{
		printf("Cant close the file\n");
	}
	if(ptr)
	{
		munmap(ptr, size);
		ptr = NULL;
		//sysdevs[idx] = NULL;
	}
}
object  dev_read_data16(object idx, int offset)
{
	unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	if(ptr)
		return newInteger(*((unsigned short*)(ptr+offset)));
	return nilobj;
}
object dev_write_data_chunk(object idx, object config, object data)
{
	/*
	This function provides an optimal way to send data to FPGA by 
	writing a chunk of data at a time (Using normal loop in Smalltalk
	degrade the performance of the communication interface).
	Read the config object to figure out how to write data to FPGA
	THe config object must contain the following elements :
		+ The address of the data register
		+ The write mode (dont know how to handle it)
		+ The other registers should be in the loop*/
	//unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	int loop_size = sizeField(data);
	//printf("data size %d\n", loop_size);
	// first instance variable is the address of the main register
	int doffset = intValue(basicAt(config,1));
	//printf("Data address %d\n", doffset);
	object dvalue;
	// the second instance variable is the list of additional registers with
	// the corresponding activated value
	int opt_registers = basicAt(config,2);
	int opt_size = opt_registers==nilobj?0:sizeField(opt_registers);
	//printf("Options size %d\n",opt_size );
	// the third instance variable is the word size
	int wsize = intValue(basicAt(config,3));
	//printf("wsize %d\n", wsize);
	// address and value of each additional register
	int daddr = basicAt(config,4)==nilobj?-1:intValue(basicAt(config,4));
	//printf("Dta addr %d\n",daddr );
	int optr_addr;
	object optr_data;
	//return falseobj;
	for(int i = 0; i < loop_size; ++i)
	{
		// first active all additional registers
		for(int j = 0; j < opt_size; ++j)
		{
			optr_addr = intValue(basicAt(basicAt(opt_registers,j+1),1));
			//printf("opadr %d\n",optr_addr );
			optr_data = basicAt(basicAt(opt_registers, j+1),2);
			dev_write_data_by_words(idx,optr_addr, optr_data, wsize);
		}
		if(daddr != -1)
			dev_write_data_by_words(idx, daddr,newInteger(i),wsize);
		dvalue = basicAt(data, i+ 1);
		// then send data to the main register
		dev_write_data_by_words(idx,doffset,dvalue, wsize);
	}
	return trueobj;
}
object dev_read_data_chunk(object idx, object config, object dvalue)
{
	/*
	TODO:
		The data could be array of Integer or ByteArray, for this version
		we fix the data size at 16 bits, but this should be replaced by 
		an auto convertion algorithm between types
	*/
	// first instance variable is the address of the main register
	int doffset = intValue(basicAt(config,1));
	//printf("Data address %d\n", doffset);
	int loop_size = sizeField(dvalue);
	// the corresponding activated value
	int opt_registers = basicAt(config,2);
	int opt_size = opt_registers==nilobj?0:sizeField(opt_registers);
	//printf("Options size %d\n",opt_size );
	// the third instance variable is the word size
	int wsize = intValue(basicAt(config,3));
	//printf("wsize %d\n", wsize);
	// address and value of each additional register
	int daddr = basicAt(config,4)==nilobj?-1:intValue(basicAt(config,4));
	//printf("Dta addr %d\n",daddr );
	int optr_addr;
	object optr_data;
	//return falseobj;
	for(int i = 0; i < loop_size; ++i)
	{
		// first active all additional registers
		for(int j = 0; j < opt_size; ++j)
		{
			optr_addr = intValue(basicAt(basicAt(opt_registers,j+1),1));
			//printf("opadr %d\n",optr_addr );
			optr_data = basicAt(basicAt(opt_registers, j+1),2);
			dev_write_data_by_words(idx,optr_addr, optr_data, wsize);
		}
		if(daddr != -1)
			dev_write_data_by_words(idx, daddr,newInteger(i),wsize);
		// we fix data size at 2 bytes, but this should be calculated using
		// the requested data size
		basicAtPut(dvalue,i+1, dev_read_by_words(idx,doffset,wsize,2));
	}
	return trueobj;
}
object dev_write_data_by_words(object idx, int offset, object data, int ws)
{
	unsigned char* ptr = ((unsigned char*) sysMemPtr(idx))+offset;
	// figure out which data should be write
	int tmp;
	if(!ptr)
	{
		printf("Cannot get the write pointer\n");
		return falseobj;
	}
	if(isInteger(data))
	{
		tmp = intValue(data);
		// write data by word size
		switch(ws)
		{
			case 8:
				*((unsigned char*)ptr) = (unsigned char)(tmp & 0xff);
				if(IS_16(tmp) || IS_32(tmp))
					*((unsigned char*)(ptr +1)) = (unsigned char)((tmp&0xff00)>>8);
				if(IS_32(tmp))
				{
					*((unsigned char*)(ptr +2)) = (unsigned char)((tmp& 0xff0000)>>16);
					*((unsigned char*)(ptr +3)) = (unsigned char)(tmp>>24);
				}
				break;
			case 16:
			//printf("Write value %d to %d\n",(tmp & 0xffff),offset);
				*((unsigned short*)ptr) = (unsigned short)(tmp & 0xffff);
				if(IS_32(tmp))
					*((unsigned short*)(ptr+2)) = (unsigned short)(tmp >> 16);
				break;
			case 32:
				*((unsigned int*)ptr) = (unsigned int)(tmp);
				break;
		}
		return trueobj;
	}
	else
	{
		printf("writing value of non integer is not supported\n");
		return falseobj;
	}
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
object dev_read_by_words(object idx, int offset, int ws, int dsize)
{
	unsigned char* ptr = (unsigned char*) sysMemPtr(idx);
	if(ptr)
	{
		switch(ws)
		{
			case 8:
				return newInteger(
									(dsize == 4)?(*((unsigned char*)(ptr+offset+3)) << 24):0 + 
									(dsize >=3)?(*((unsigned char*)(ptr+offset+2)) << 16):0 +
									(dsize >= 2)?(*((unsigned char*)(ptr+offset+1)) <<8):0  +
									(*((unsigned char*)(ptr+offset))));
			case 16:
				return newInteger( ((dsize == 4)?((int)*((unsigned short*)(ptr+offset+2)) << 16):0) +
									((int)*((unsigned short*)(ptr+offset))));
			case 32:
				return newInteger(*((int*)(ptr+offset)));
		}
	}
	return nilobj;
}
object dev_wait_for_irq(object obj)
{
	/*TODO: this didn't work, must figure out why?
		the read function didn't stop when there is an IRQ,
		this is not a normal behaviour
	*/
	int devf = obj >> 1;
	if(devf <0)
	{
    	return falseobj;
	}
	uint32_t info;
	ssize_t nb = read(devf,&info,sizeof(info));
	if(nb == sizeof(info))
	{
		return trueobj;
	}
	return falseobj;
}
/**
	cofigure the fpga with given file
	
*/
object dev_conf_fpga(object file, object module)
{
	char* path = charPtr(file);
	char* mpath = charPtr(module);
	//printf("Load gateware of : %s with kernel %s\n", path, mpath);
	//return trueobj;
	char cmd[255];
	int status;
	// check if the file exist
	if(!FILE_OK(path)) return falseobj;
	// remove the kernel
	sprintf(cmd,"modprobe -rf %s",mpath);
	status = system(cmd);
	if(status != 0 ) printf("Problem when remove the kernel module : %s", mpath);
	// configure the fpga
	sprintf(cmd,"load_fpga %s", path);
	status = system(cmd);
	if(status != 0)
	{
		printf("Cannot program the fpga %s", path);
		return falseobj;
	}
	// reinsert the module
	sprintf(cmd,"modprobe %s", mpath);
	status = system(cmd);
	if(status != 0)
	{
		printf("cannot re-activate the kernel module");
		return falseobj;
	}
	return trueobj;
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
			dev_close(idx,data, args[3]>>1);
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
		case DEV_R_BY_W:
			return dev_read_by_words(args[1], (args[2]>>1)-1, args[3] >> 1,args[4]>>1);
		case DEV_IRQ:
			return  dev_wait_for_irq(args[1]);
		case DEV_W_BY_W:
			return dev_write_data_by_words(args[1],(args[2]>>1)-1, args[3], args[4]>>1);
		case DEV_W_CHUNK:
			return dev_write_data_chunk(args[1], args[2], args[3]);
		case DEV_R_CHUNK:
			return dev_read_data_chunk(args[1], args[2], args[3]);
		case DEV_CONF_FPGA:
			return dev_conf_fpga(args[1], args[2]);
		default: 
			printf("Unsupported device primitive:%d\n",type );
			return nilobj;
			break;
	}
}
