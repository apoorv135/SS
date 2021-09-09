/* 
MT2020013 Apoorv Panse
Write a program to copy file1 into file2.
*/

#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(){
    const char *sourceFile = "test7_source.txt";
    const char *destinationFile = "test7_destination.txt";

    int sourceFileFd = open(sourceFile, O_RDONLY, 0777);

    if(sourceFileFd == -1){
        printf("sourceFile: %s\n",strerror(errno));
    }

    int destinationFileFd = open(destinationFile, O_TRUNC | O_WRONLY | O_CREAT| O_EXCL, 0777);
    if(destinationFileFd == -1){
        printf("Destination: %s\n",strerror(errno));
    }
    else{
        char buff[1];
        while(read(sourceFileFd, buff, sizeof buff) > 0){
            write(destinationFileFd, buff, sizeof buff);
        }
        write(1, "File copied successfully...\n", 29);
    }

    close(destinationFileFd);
    close(sourceFileFd);
    return 0;
}