/* 
MT2020013 Apoorv Panse
23. Write a program to print the maximum number of files can be opened within a process and
size of a pipe (circular buffer).
*/
#include<unistd.h>
#include<stdio.h>

int main()
{
  long long maxFiles=sysconf(_SC_OPEN_MAX);
  printf("Maximum number of files that can be opened within a process: %lld\n",maxFiles);

  long long pipeSize=pathconf("test23",_PC_PIPE_BUF);
  printf("Size of pipe test23: %lld\n", pipeSize);

    return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out 
Maximum number of files that can be opened within a process: 1024
Size of pipe test23: 4096
*/