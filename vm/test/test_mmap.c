#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAP_SIZE 5120
#define DEV_F "dummy"

int main (int argc, char *argv[])
{
	int devf = open(DEV_F, O_RDWR|O_SYNC);
	unsigned char* ptr;
	if(devf <0)
	{
    	printf("%s","Cannot open dummy file\n");
    	return 1;
	}
	ptr = mmap(0,MAP_SIZE,PROT_READ|PROT_WRITE, MAP_SHARED, devf, 0);
	if(ptr == MAP_FAILED)
	{
		ptr = NULL;
    	printf("%s","MMap faile\n");
    	return 1;
	}
	// close the file
	if(close(devf) == -1)
	{
		printf("Cant close the file\n");
		return 1;
	}
	for(size_t i = 0; i < MAP_SIZE; ++i)
		*(ptr+i) = i%255;
	
	// read file
	for(size_t i = 0; i < MAP_SIZE; ++i)
	{
		printf("[%d] ", *(ptr+i));
	}
	// unmap
	munmap(ptr, MAP_SIZE);
}