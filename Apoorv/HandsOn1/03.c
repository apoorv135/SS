/* 
MT2020013 Apoorv Panse
Write a program to create a file and print the file descriptor value. Use creat ( ) system call
*/

#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(){
    const char * file = "test.txt";
    int fileDescriptor = creat(file, 0777);
    if(fileDescriptor == -1){
        printf("%s\n",strerror(errno));
    }
    else{
        printf("File descriptor: %d \n", fileDescriptor);
    }
    return 0;
}