#include<stdio.h>
int main()
{
	char *ptr = "hello world";
	*ptr = 'H';
	printf("%s\n",ptr);
}
