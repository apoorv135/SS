/* 
MT2020013 Apoorv Panse
9. Write a program to ignore a SIGINT signal then reset the default action of the SIGINT
signal - Use signal system call.
*/

#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

void sigHandler(int sigNum)
{  
    printf("SIGINT is handled \n");
    printf("SIGINT number : %d\n",sigNum);
    exit(0);
}

int main()
{

	signal(SIGINT, SIG_IGN);
	printf("SIGINT is being ignored for 5 seconds\n");
	sleep(5);
	printf("\n");
	signal(SIGINT, SIG_DFL);
	printf("Default action for SIGINT enabled, press ctrl+c within 10 seconds\n");
    signal(SIGINT, sigHandler);
	sleep(10);
	printf("No SIGINT within 10 sec \n\n");
	return 0;

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
SIGINT is being ignored for 5 seconds
^C
Default action for SIGINT enabled, press ctrl+c within 10 seconds
^CSIGINT is handled 
SIGINT number : 2
*/