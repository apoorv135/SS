/* 
MT2020013 Apoorv Panse
Write a program to create five new files with infinite loop. Execute the program in the background
and check the file descriptor table at /proc/pid/fd.
*/

#include <stdio.h>
#include <fcntl.h>    
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
    
	int fd1 = open("test1_5", O_RDWR | O_CREAT, 0777);
	int fd2 = open("test2_5", O_RDWR | O_CREAT, 0777);
	int fd3 = open("test3_5", O_RDWR | O_CREAT, 0777);
	int fd4 = open("test4_5", O_RDWR | O_CREAT, 0777);
	int fd5 = open("test5_5", O_RDWR | O_CREAT, 0777);
	if(fd1 < 0 || fd2 < 0 || fd3 < 0 || fd4 < 0 || fd5 < 0)
	{
		printf("%s\n",strerror(errno));
	}
	printf("File descriptor of the opened test1_5: %d\n", fd1);
	printf("File descriptor of the opened test2_5: %d\n", fd2);
	printf("File descriptor of the opened test3_5: %d\n", fd3);
	printf("File descriptor of the opened test4_5: %d\n", fd4);
	printf("File descriptor of the opened test5_5: %d\n", fd5);
	while(1);
    
    return 0;

}