/* 
MT2020013 Apoorv Panse
Write a program to open a file in read only mode, read line by line and display each line as it is read.
Close the file when end of file is reached.
*/

#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void main(){
    const char *targetFile = "08.c";

    int targetFileDescriptor = open(targetFile, O_RDONLY);
    if(targetFileDescriptor == -1){
        printf("%s\n",strerror(errno));
    }

    char buff[1];
    while(read(targetFileDescriptor, buff, sizeof buff) > 0){
        write(1, buff, sizeof buff);
    }
    close(targetFileDescriptor);
}