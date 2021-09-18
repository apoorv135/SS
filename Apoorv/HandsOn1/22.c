/* 
MT2020013 Apoorv Panse
Write a program, open a file, call fork, and then write to the file by both the child as well as the
parent processes. Check output of the file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main()
{
	int fd = open("test22.txt", O_WRONLY | O_CREAT, 0777);

    if(fd < 0){
        printf("%s\n",strerror(errno));
    }

	if(!fork())
	{
		write(fd, "Child writing\n", 14);
	}
	else
	{
		write(fd, "Parent writing\n", 15);
	}
	return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn1$ cat test22.txt
Child writing
Parent writing

*/