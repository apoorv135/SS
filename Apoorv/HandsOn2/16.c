/* 
MT2020013 Apoorv Panse
16. Write a program to send and receive data from parent to child vice versa. Use two way
communication.
*/

#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
    int parentFD[2], childFD[2];
    pipe(parentFD);
    pipe(childFD);
    if(!fork()){
        close(parentFD[1]);
        close(childFD[0]);
        char buff[19];
        read(parentFD[0], buff, 19);
        printf("Data received from parent: %s", buff);
        close(parentFD[0]);
        write(childFD[1], "Hello from child\n", 18);
        close(childFD[1]);
        
    }
    else{
        close(parentFD[0]);
        close(childFD[1]);

        write(parentFD[1], "Hello from parent\n", 19);
        close(parentFD[1]);
        char buff[18];
        read(childFD[0], buff, 18);
        printf("Data received from child: %s", buff);
        close(childFD[0]);
        
        
    
    }
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Data received from parent: Hello from parent
Data received from child: Hello from child
*/