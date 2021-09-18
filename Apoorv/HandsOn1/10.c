/* 
MT2020013 Apoorv Panse
Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
a. check the return value of lseek
b. open the file with od and check the empty spaces in between the data.
*/

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>    
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {

    int fd = open("test10.txt", O_CREAT | O_RDWR | O_EXCL, 0777);

	if(fd < 0){
        printf("%s\n",strerror(errno));
    }

    if(write(fd, "HelloWorld", 10) < 0){
        printf("%s\n",strerror(errno));
    }
    
    int return_val = lseek(fd, 10, SEEK_CUR);

    if(return_val < 0){
        printf("%s\n",strerror(errno));
    }

    printf("The return value of lseek() = %d\n", return_val);

    if(write(fd, "HeyWorld!!", 10) < 0){
        printf("%s\n",strerror(errno));
    }
    
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn1$ ./10.sh
The return value of lseek() = 20
Output of od : \n
0000000   H   e   l   l   o   W   o   r   l   d  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0   H   e   y   W   o   r   l   d   !   !
0000036

*/