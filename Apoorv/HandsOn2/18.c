/* 
MT2020013 Apoorv Panse
18. Write a program to find out total number of directories on the pwd.
execute ls -l | grep ^d | wc ? Use only dup2.
*/

#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    int firstPipe[2];
    int secondPipe[2];
    pipe(firstPipe);
    pipe(secondPipe);

    if(!fork()){
        dup2(firstPipe[1], 1);
        close(firstPipe[0]);
        close(secondPipe[0]);
        close(secondPipe[1]);
        execl("/bin/ls", "ls", "-l", NULL);
    }
    else{
        if(!fork()){
            close(firstPipe[1]);
            close(secondPipe[0]);
            dup2(firstPipe[0], 0);
            dup2(secondPipe[1], 1);
            execl("/bin/grep", "grep", "^d", NULL);
        }
        else{
            close(firstPipe[1]);
            close(firstPipe[0]);
            close(secondPipe[1]);
            dup2(secondPipe[0], 0);
            execl("/usr/bin/wc", "wc", NULL);
        }
    }
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ gcc 18.c
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv$ cd ..
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv$ ls -lrt
total 5832
-rw-rw-rw- 1 apoorv apoorv  424747 Jan  3  2021 'Hands-on List II.pdf'
-rw-rw-rw- 1 apoorv apoorv  429969 Jan  3  2021 'Hands-on List 1.pdf'
-rw-rw-rw- 1 apoorv apoorv 4569524 Aug 26 09:39 'CS 513 Software Systems.pptx'
-rw-rw-rw- 1 apoorv apoorv  529131 Aug 31 08:33  MiniProject.pdf
drwxrwxrwx 2 apoorv apoorv    4096 Sep  8 19:06 'Prep Term Course Materials'
drwxrwxrwx 2 apoorv apoorv    4096 Oct  7 14:53  HandsOn1
drwxrwxr-x 2 apoorv apoorv    4096 Oct 11 21:09  HandsOn2
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv$ ./HandsOn2/a.out
      3      30     189
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv$ ls -l | grep ^d | wc
      3      30     189
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv$ 
*/