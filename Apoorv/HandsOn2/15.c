/* 
MT2020013 Apoorv Panse
15. Write a simple program to send some data from parent to the child process.
*/
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
    int fd[2];
    pipe(fd);
    if(!fork()){
        
        sleep(1);
        char buff[11];
        read(fd[0], buff, 11);
        printf("Data recieved from parent: %s", buff);
        close(fd[0]);
        close(fd[1]);
    }
    else{
        
        write(fd[1], "HelloWorld\n", 11);
        close(fd[0]);
        close(fd[1]);
        sleep(2);
    }
    return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ gcc 15.c
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Data recieved from parent: HelloWorld

*/