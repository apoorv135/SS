/* 
MT2020013 Apoorv Panse
20. Write two programs so that both can communicate by FIFO -Use one way communication.
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    int fd;
    char * myfifo = "test20";
    char buffer[100];

    while (1)
    {        
        fd = open(myfifo,O_RDONLY);
        if(fd < 0){
            printf("FIFO terminated or does not exist\n");
            close(fd);
            exit(0);
        }
        if(read(fd, buffer, 100) > 0 )
            printf("Program 1 entered : %s\n", buffer);
        close(fd);


        fd = open(myfifo, O_WRONLY);
        if(fd < 0){
            printf("FIFO terminated or does not exist\n");
            exit(0);
        }             
        printf("Write Something or enter \"exit\" : ");
        fgets(buffer, 100, stdin);
        if(strcmp(buffer,"exit\n") == 0) {    
            unlink(myfifo);
            close(fd);
            exit(0);
        }
        write(fd, buffer, strlen(buffer)+1);
        close(fd);
        
    }
    return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:/home/apoorv/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./21b
Program 1 entered : Hello from P1

Write Something or enter "exit" : Hello from P2
Program 1 entered : Hello from P1 again

Write Something or enter "exit" : Hello from P2 again
FIFO terminated or does not exist

*/