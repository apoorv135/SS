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

    fd = open(myFile, O_WRONLY | O_CREAT, 0777);
    if(fd == -1){
        printf("%s\n",strerror(errno));
    }
    printf("Enter a message to send: ");
    fgets(buffer, 100, stdin);
    write(fd, buffer, sizeof(buffer));
    close(fd);
    return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./22a
Enter a message to send: This message is written by Program 1

*/