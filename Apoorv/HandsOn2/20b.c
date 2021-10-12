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
    char buff[100];

    while (1)
    {        
        fd = open(myfifo,O_RDONLY);
        if(fd < 0){
            printf("FIFO terminated or does not exist\n");
            close(fd);
            exit(0);
        }
        if(read(fd, buff, 100) > 0 )
            printf("Read from FIFO: %s\n", buff);
        
    }
    return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:/home/apoorv/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./20b
Read from FIFO: Apoorv Panse

Read from FIFO: MT2020013

FIFO terminated or does not exist
*/