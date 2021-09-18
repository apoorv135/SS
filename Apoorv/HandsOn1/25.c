/*
MT2020013 Apoorv Panse
Write a program to create three child processes. The parent should wait for a particular child (use
waitpid systemcall).
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{   
    int pid1, pid2, pid3;
    
    printf("Parent waits for 3rd child...\n");

    pid1 = fork();
    if(pid1 > 0){
        pid2 = fork();
        if(pid2 > 0){
            pid3 = fork();
            if(pid3 > 0){
                printf("parent waiting for: %d\n", pid3);
                waitpid(pid3, NULL, 0);
                printf("Parent exiting...\n");
            }
            else{
                sleep(5);
                printf("Child 3 exiting...\n");
            }
        }
        else{
            sleep(7);
            printf("Child 2 exiting...\n");
        }
    }
    else{
        printf("Child 1 exiting...\n");
    }
	
	return 0;
}

/*
Parent waits for 3rd child...
Child 1 exiting...
parent waiting for: 15881
Child 3 exiting...
Parent exiting...
Child 2 exiting...
*/