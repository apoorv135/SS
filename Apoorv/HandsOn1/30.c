/*
MT2020013 Apoorv Panse
Write a program to run a script at a specific time using a Daemon process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define SLEEP_TIME 6

int main()
{
	if(!fork()) {
        printf("Child pid : %d\n", getpid());
        setsid();         
        umask(0); 
        while(1) {
            sleep(SLEEP_TIME);
            system("bash test30.sh");
        }
    } else exit(0);
    return 0;
}
