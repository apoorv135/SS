/* 
MT2020013 Apoorv Panse
Write a program to find the type of a file.
a. Input should be taken from command line.
b. program should be able to identify any type of a file.
*/

#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <stdio.h>
#include <fcntl.h>    
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
	struct stat stats;
	if(lstat(argv[1], &stats) < 0 ) 
		printf("%s\n",strerror(errno));

	printf("File type: ");

	switch(stats.st_mode & S_IFMT)
	{
		case S_IFBLK: printf("block device\n"); break;
		case S_IFCHR: printf("character device\n"); break;
		case S_IFDIR: printf("directory\n"); break;
		case S_IFIFO: printf("FIFO/pipe\n"); break;
		case S_IFLNK: printf("symlink\n"); break;
		case S_IFREG: printf("regular file\n"); break;
		case S_IFSOCK: printf("socket\n"); break;
		default:	  printf("Unknown\n");
	}
	exit(EXIT_SUCCESS);
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn1$ gcc 14.c -o 14.out 
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn1$ ./14.out 14.c
File type: regular file
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn1$ ./14.out .
File type: directory


*/