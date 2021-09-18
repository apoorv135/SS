/* 
MT2020013 Apoorv Panse
Write a program to open a file, duplicate the file descriptor and append the file with both the
descriptors and check whether the file is updated properly or not.
a. use dup
b. use dup2
c. use fcntl
*/

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>    
#include <unistd.h>


int main() {
    int fd, dfd;    
    fd = open("test11a.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    printf("Duplicating the file descriptor using dup for file descriptor %d\n", fd);

    dfd = dup(fd);    
    printf("dup return value is %d\n", dfd);
    
    write(fd, "Writing this via normal fd\n", 27);
    write(dfd, "writing this using dup", 22);

    close(fd);
    return 0;
}