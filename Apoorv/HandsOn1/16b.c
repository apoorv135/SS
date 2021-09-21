/* 
MT2020013 Apoorv Panse
Write a program to perform mandatory locking.
a. Implement read lock
b. Implement write lock
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{

    struct flock wLock;
    int fd;
    char *buffer = "This is content ! ";


    fd = open("test16b.txt",O_CREAT|O_EXCL|O_RDWR, 0777);

    printf("Getting writeLock...\n");
    wLock.l_type = F_WRLCK;
	wLock.l_whence = SEEK_CUR;
	wLock.l_start = 0;
	wLock.l_len = 0;
	wLock.l_pid = getpid();

    printf("About to enter the critical section...\n");
    fcntl(fd,F_SETLKW,&wLock);

    printf("Inside Critical section...\n");
    write(fd,buffer,sizeof(buffer));

    printf("Unlocking....\n");
    wLock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK,&wLock);

	printf("Unlocked....\n");

 	return 0;
}