#include<stdio.h>
#include <stdlib.h>
int main()
{	
	int *p=malloc(sizeof(int));
	*p=7;
	printf("Hello World\n");
	free(p);
	p=NULL;
	return 0;
}
