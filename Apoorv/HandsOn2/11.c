/* 
MT2020013 Apoorv Panse
11. Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal -
use sigaction system call.
*/

#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>


int main()
{
	struct sigaction action;
	action.sa_handler=SIG_IGN;
	action.sa_flags=SA_RESTART; 	
	
	sigaction(SIGINT, &action, NULL); 
	
	printf("Generate an Interrupt\n");
	sleep(5);
	printf("\nInterrupt was Ignored\n");	
	
	
	action.sa_handler=SIG_DFL; 
	sigaction(SIGINT, &action, NULL);	
	
	printf("Generate an Interrupt\n");
	sleep(5);
	printf("No interrupt was generated within 5 seconds\n");
	return 0;

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Generate an Interrupt
^C
Interrupt was Ignored
Generate an Interrupt
^C

*/