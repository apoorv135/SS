/* 
MT2020013 Apoorv Panse
1. Create the following types of a files using (i) shell command (ii) system call
c. FIFO (mkfifo Library Function or mknod system call)
*/
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <errno.h>
#include <string.h>
 //System call mkfifo and mknod
int main(){

    const char *targetFile = "test.txt";
    const char *mkfifotest = "1c_mkfifotest.txt";
    const char *mknodtest = "1c_mknodtest.txt";

    int fileDescriptor;
    fileDescriptor=open (targetFile,O_CREAT| O_WRONLY| O_TRUNC, 0777 );
    
    if(mkfifo(mkfifotest, 0777) == 0){
                            printf("Named Pipe created successfully using mkfifo...\n");
                        }
                        else {
                            printf("%s\n",strerror(errno));
                        }
    if(mknod(mknodtest, 0777, makedev(12,3)) == 0){
                            printf("Named Pipe created successfully using mknod...\n");
                        }
                        else {
                            printf("%s\n",strerror(errno));
                        }
    close(fileDescriptor);
    return 0;
}



