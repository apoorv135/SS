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
#include <sys/types.h>

int main()
{
   pid_t pid;
   printf("Enter PID of the process to stop: ");
   scanf("%d",&pid);
   sleep(1);
   printf("Stopped process %d\n",pid);
   kill(pid, SIGSTOP);
   return 0;
}