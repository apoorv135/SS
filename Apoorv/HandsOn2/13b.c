/* 
MT2020013 Apoorv Panse
13. Write two programs: first program is waiting to catch SIGSTOP signal, the second program
will send the signal (using kill system call). Find out whether the first program is able to catch
the signal or not.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigHandler(int sigNum)
{  
    printf("SIGSTOP is handled \n");
    printf("SIGSTOP number : %d\n",sigNum);
    exit(0);
}

int main(void)
{   
    printf("My PID: %d\n",getpid());
    printf("Waiting for other process to call kill()...\n");
    signal(SIGSTOP, sigHandler);
    while(1);
   
   return 0;

}

/*
===================TERMINAL 1============================
apoorv@apoorv-GV62VR-7RF:/home/apoorv/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./13b
My PID: 8081
Waiting for other process to call kill()...

[1]+  Stopped                 ./13b

===================TERMINAL 2============================
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./13a
Enter PID of the process to stop: 8081
Stopped process 8081

Kill signal cannot be caught if sent from another process.
*/
