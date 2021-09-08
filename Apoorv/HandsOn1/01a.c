/* 
MT2020013 Apoorv Panse
1. Create the following types of a files using (i) shell command (ii) system call
a. soft link (symlink system call)
*/
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
 //System call for soft link
int main(){

    const char *targetFile = "test.txt";
    const char *softlink = "1a_softlink.txt";

    int fileDescriptor;
    fileDescriptor=open ("test.txt",O_CREAT| O_WRONLY| O_TRUNC, 0777 );
    
    if(symlink(targetFile, softlink) == 0){
                            printf("soft link created successfully...\n");
                        }
                        else {
                            printf("%s\n",strerror(errno));
                        }
    close(fileDescriptor);
    return 0;
}



