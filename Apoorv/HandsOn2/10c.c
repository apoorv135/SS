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
	printf("SIGFPE is handled \n");
    printf("SIGFPE number : %d\n",sigNum);
	exit(0);
}


int main()
{
	struct sigaction action;
	action.sa_handler=sigHandler; 
	action.sa_flags=0;
	
    sigaction(SIGFPE, &action, NULL); 
	
	printf("Generating SIGFPE..\n");
	sleep(1);
	
	int i=10/0;

	printf("Normal execution without any error\n");
	
	return 0;

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Generating SIGFPE..
SIGFPE is handled 
SIGFPE number : 8


*/