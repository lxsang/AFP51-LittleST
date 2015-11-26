#include "../source/free_list.h"

int main (int argc, char const *argv[])
{
	
	if(!FL_SET(0,15)) {printf("Cannot put value\n");};
	if(!FL_SET(1,16)) {printf("Cannot put value\n");};
	if(!FL_SET(2,17)) {printf("Cannot put value\n");};
	if(!FL_SET(1024,34)) {printf("Cannot put value\n");};
	if(!FL_SET(1025,47)) {printf("Cannot put value\n");};
	if(!FL_SET(2,40)) {printf("Cannot put value\n");};
	printf("Direct access %d\n", objectFreeList[2]->value);
	printf("GET object of size 2 %d\n", FL_GET(2));
	oslot_t np;
	printf("dump all value \n");
	for_each_slot(np)
	{
		printf("Found %d\n", np->value);
	}
	return 0;
}