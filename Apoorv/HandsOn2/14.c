/* 
MT2020013 Apoorv Panse
14. Write a simple program to create a pipe, write to the pipe, read from pipe and display on
the monitor.
*/

#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
    int fd[2];
    pipe(fd);

    write(fd[1], "HelloWorld\n", 11);
    char buff[11];
    read(fd[0], buff, 11);
    printf("Read: %s", buff);

    close(fd[0]);
    close(fd[1]);
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ gcc 14.c
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Read: HelloWorld
*/