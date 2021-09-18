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
#include <string.h>
#include <errno.h>

int main() {

    int fd;
    fd = open("test11c.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);

    printf("Duplicating the file descriptor using fcntl for file descriptor %d\n", fd);
    int ffd = fcntl(fd, F_DUPFD, fd);

    if(ffd < 0){
        printf("%s\n",strerror(errno));
    }
    printf("ffd is: %d\n", ffd);

    if(write(ffd, "Written using ffd duplicate fd created by fcntl().\n", 51) < 0){
        printf("%s\n",strerror(errno));
    }
    
    return 0;
}