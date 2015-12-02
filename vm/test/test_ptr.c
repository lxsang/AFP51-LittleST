#include "stdio.h"
int main (int argc, char const *argv[])
{
	char* c = "hello";
	printf("Address %d\n",&c);
	char **p = &c;
	printf("value p %d\n",p);
	printf("Value ref %s\n", *p);
	c = "another string";
	printf("Value ref %s\n", *p);
	return 0;
}