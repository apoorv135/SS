/* 
MT2020013 Apoorv Panse
10. Write a separate program using sigaction system call to catch the following signals.
a. SIGSEGV
b. SIGINT
c. SIGFPE
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
	struct sigaction action;
	action.sa_handler=sigHandler; 
	action.sa_flags=0;
	
	sigaction(SIGINT, &action, NULL); 
	
	printf("Generate Interrupt!\n");
	sleep(5);	
	printf("No interrupt generated within 5 seconds\n");
	
	return 0;

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Generate Interrupt!
^CSIGINT is handled 
SIGINT number : 2

*/