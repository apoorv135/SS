/* 
MT2020013 Apoorv Panse
Write a program to create a Zombie state of the running program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	if(!fork()) {
        printf("Terminating Child process with process id : %d\n", getpid());
    }

	else
	{
		printf("PID of parent: %d\n", getpid());
		printf("Press any key to terminate the parent process.\n");
		getchar();
	}
	return 0;
}

/*
PID of parent: 12807
Press any key to terminate the parent process.
Terminating Child process with process id : 12812
*/
