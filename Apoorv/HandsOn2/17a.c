/* 
MT2020013 Apoorv Panse
17. Write a program to execute ls -l | wc.
a. use dup
b. use dup2
c. use fcntl
*/

#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    int fd[2];
    pipe(fd);

    if(!fork()){
        close(1); 
        dup(fd[1]);
        close(fd[0]);
        execl("/bin/ls", "ls", "-l", NULL);
    }
    else{
        sleep(0.5);
        close(0);
        close(fd[1]);
        dup(fd[0]);
        execl("/usr/bin/wc", "wc", NULL);
    }
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ls -l | wc
     32     281    1610
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ gcc 17a.c
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
     32     281    1610

*/