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

    struct flock rLock;
    int fd;
    char content[100];

    fd = open("test16a.txt",O_CREAT|O_EXCL|O_RDWR, 0777);
    write(fd,"SampleText",10);
    
    printf("Getting readLock...\n");
    rLock.l_type = F_RDLCK;
	rLock.l_whence = SEEK_CUR;
	rLock.l_start = 0;
	rLock.l_len = 0;
	rLock.l_pid = getpid();

    printf("About to enter the critical section...\n");
    fcntl(fd,F_SETLKW,&rLock);

    fd = open("test16a.txt",O_CREAT|O_EXCL|O_RDWR, 0777);
    printf("Inside Critical section...\n");
    read(fd,content,100);
    printf("File Content : %s\n",content);
    
    printf("Unlocking...\n");
    rLock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK,&rLock);

	printf("Unlocked...\n");

 	return 0;
}