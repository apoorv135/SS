/* 
MT2020013 Apoorv Panse
Write a program, call fork and print the parent and child process id.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	if(!fork())
	{
		printf("Process id of child: %d\n", getpid());
	}
	else
	{
		printf("Process id of parent: %d\n", getpid());
	}
	return 0;
}