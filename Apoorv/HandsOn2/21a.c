/* 
MT2020013 Apoorv Panse
21. Write two programs so that both can communicate by FIFO -Use two way communications.
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
    mkfifo(myfifo, 0777);
    
    while (1)
    {   fd = open(myfifo, O_WRONLY);
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

        fd = open(myfifo,O_RDONLY);
        if(fd < 0){
            printf("FIFO terminated or does not exist\n");
            close(fd);
            exit(0);
        }
        if(read(fd, buffer, 100) > 0 )
            printf("Program 2 entered : %s\n", buffer);
                
    }
    
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./21a
Write Something or enter "exit" : Hello from P1
Program 2 entered : Hello from P2

Write Something or enter "exit" : Hello from P1 again
Program 2 entered : Hello from P2 again

Write Something or enter "exit" : exit

*/