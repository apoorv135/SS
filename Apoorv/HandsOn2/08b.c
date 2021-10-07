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
    printf("SIGINT is handled \n");
    printf("SIGINT number : %d\n",sigNum);
    exit(0);
}

int main(void)
{
    int i;
    signal(SIGINT, sigHandler);
    sleep(5);
    printf("No SIGINT in last 5 seconds\n");
    return 0;

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
No SIGINT in last 5 seconds
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
^CSIGINT is handled 
SIGINT number : 2

*/