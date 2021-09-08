/* 
MT2020013 Apoorv Panse
Write a program to open an existing file with read write mode. Try O_EXCL flag also.
*/

#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(){
    const char *file = "test4.txt";
    int fileDescriptor = open(file,O_RDWR | O_EXCL | O_CREAT);
    if(fileDescriptor == -1){
        printf("%s\n",strerror(errno));
    }
    else{
        printf("File opened in read write mode \n");
    }
    close(fileDescriptor);
    return 0;
}