
/* 
MT2020013 Apoorv Panse
Write a program to find out the opening mode of a file. Use fcntl.
*/

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>    
#include <unistd.h>
#include <string.h>
#include <errno.h>


int main() {

    int fd = open("test12.txt", O_CREAT | O_RDONLY | O_EXCL, 0777);
	if(fd < 0){
        printf("%s\n",strerror(errno));
    }

	int flag = fcntl(fd, F_GETFL);
	if(flag < 0){
        printf("%s\n",strerror(errno));
    }

	printf("Opening mode is : ");
	
	if((flag & O_ACCMODE) == O_RDONLY) 
		printf("Read Only Mode\n");

	if((flag & O_ACCMODE) == O_WRONLY) 
		printf("Write Only Mode\n");
	
	if((flag & O_ACCMODE) == O_RDWR)
		printf("Read & Write Mode\n");
		
	close(fd);
    return 0;
}