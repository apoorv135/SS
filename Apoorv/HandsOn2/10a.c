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
	printf("SIGSEGV is handled \n");
    printf("SIGSEGV number : %d\n",sigNum);
	exit(0);
}


int main()
{
	struct sigaction action;
	action.sa_handler=sigHandler; 
	action.sa_flags=0;
	int i;
	
	sigaction(SIGSEGV, &action, NULL);
	printf("Enter a number : ");
	scanf("%d",i);  
	
	printf("Normal execution without any SIGNAL\n");
	
	return 0;

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Enter a number : 1
SIGSEGV is handled 
SIGSEGV number : 11

*/