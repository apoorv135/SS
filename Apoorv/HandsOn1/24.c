/*
MT2020013 Apoorv Panse
Write a program to create an orphan process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	if(!fork()) {

        printf("Child's PPID before parent is closed: %d\n", getppid());
        sleep(3);
        printf("Child's PPID after parent is closed: %d\n", getppid());

    } else {
        printf("PID of parent: %d\n", getpid());
        sleep(2);
    }
	return 0;
}

/*
PID of parent: 12421
Child's PPID before parent is closed: 12421
Child's PPID after parent is closed: 1553
*/