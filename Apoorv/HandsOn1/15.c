/* 
MT2020013 Apoorv Panse
Write a program to display the environmental variable of the user (use environ).
*/

#include <stdio.h>
#include <unistd.h>

extern char **environ;

int main()
{
	for (int i = 0; environ[i]; i++)
		printf("%s\n", environ[i]);
	return 0;
}