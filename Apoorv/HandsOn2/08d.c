/* 
MT2020013 Apoorv Panse
8. Write a separate program using signal system call to catch the following signals.
a. SIGSEGV
b. SIGINT
c. SIGFPE
d. SIGALRM (use alarm system call)
e. SIGALRM (use setitimer system call)
f. SIGVTALRM (use setitimer system call)
g. SIGPROF (use setitimer system call)
*/

#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

void sigHandler(int sigNum)
{  
    printf("SIGALRM is handled \n");
    printf("SIGALRM number : %d\n",sigNum);
    exit(0);
}

int main(void)
{
    printf("Calling alarm system call\n");
    signal(SIGALRM, sigHandler);
    alarm(1);
    pause(); 
    return 0;

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Calling alarm system call
SIGALRM is handled 
SIGALRM number : 14
*/