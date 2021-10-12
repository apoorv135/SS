/* 
MT2020013 Apoorv Panse
22. Write a program to wait for data to be written into FIFO within 10 seconds, use select
system call with FIFO.
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main()
{   
    char * myFile = "test22";
    int fd;
    char buffer[100];

    fd = open(myFile, O_RDONLY);
    if(fd == -1){
        printf("%s\n",strerror(errno));
    }

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    int retval = select(fd+1, &rfds, NULL, NULL, &tv);

    if (retval == -1)
        printf("%s\n",strerror(errno));
    else if (retval){
        read(fd, buffer, sizeof buffer);
        printf("Reading from Program 22a: %s\n", buffer);
    }
    else
        printf("No data written by Program 22a\n");
    return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:/home/apoorv/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./22b
Reading from Program 22a: This message is written by Program 1

*/