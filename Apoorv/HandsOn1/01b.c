/*
MT2020013 Apoorv Panse
1. Create the following types of a files using (i) shell command (ii) system call
b. Hard link (link system call)
*/
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
 //System call for hard link
int main(){

    const char *targetFile = "test.txt";
    const char *hardLink = "1b_hardlink.txt";

    int fileDescriptor;
    fileDescriptor=open ("test.txt",O_CREAT| O_WRONLY| O_TRUNC, 0777 );
    
    if(link(targetFile, hardLink) == 0){
                            printf("Hard link created successfully...\n");
                        }
                        else {
                            printf("%s\n",strerror(errno));
                        }
    close(fileDescriptor);
                        
    return 0;
}



