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
        dup2(fd[1], 1);
        close(fd[0]);
        execl("/bin/ls", "ls", "-l", NULL);
    }
    else{
        sleep(0.1);
        close(fd[1]);
        dup2(fd[0], 0);
        execl("/usr/bin/wc", "wc", NULL);
    }
    return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ls -l | wc
     32     281    1610
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ gcc 17b.c
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
     32     281    1610

*/