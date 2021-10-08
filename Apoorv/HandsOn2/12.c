/* 
MT2020013 Apoorv Panse
12. Write a program to create an orphan process. Use kill system call to send SIGKILL signal to
the parent process from the child process.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
  pid_t pid=fork();
  
  if(pid==0)                   
  {
   	    pid_t ppid=getppid();   
        printf("Original PPID: %d\n",ppid);
    	kill(ppid,SIGKILL);       
       
        sleep(1);
        printf("New PPID after killing Original Parent :%d\n", getppid());
  }
  else                          
  {
        printf("Parent running...\n");
        sleep(1);
  }

  return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Parent running...
Original PPID: 6532
Killed
New PPID after killing Original Parent :1607
*/