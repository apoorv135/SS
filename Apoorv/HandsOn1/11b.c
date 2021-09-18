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
    int fd = open("test11b.txt", O_CREAT | O_WRONLY | O_APPEND, 0777);
    dup2(fd, 1);
    printf("This will be printed in test11b.txt");
}