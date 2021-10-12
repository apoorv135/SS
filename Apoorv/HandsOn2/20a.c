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
    mkfifo(myfifo, 0777);
    fd = open(myfifo, O_WRONLY);
    if(fd < 0){
        printf("%s\n",strerror(errno));
    }
    while (1)
    {                
        printf("Write Something or enter \"exit\" : ");
        fgets(buffer, 100, stdin);
        if(strcmp(buffer,"exit\n") == 0) {    
            unlink(myfifo);
            close(fd);
            exit(0);
        }
        write(fd, buffer, strlen(buffer)+1);
                
    }
    
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./20a
Write Something or enter "exit" : Apoorv Panse
Write Something or enter "exit" : MT2020013
Write Something or enter "exit" : exit
*/