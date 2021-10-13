/* 
MT2020013 Apoorv Panse
31. Write a program to create a semaphore and initialize value to the semaphore.
a. create a binary semaphore
b. create a counting semaphore
*/

#include <stdio.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h> 

int main(){

    int binarySemId, countingSemId;
    int key;

    union semun{
        int val;
        struct semid_ds *buf;
        short *array;
    };
    union semun arg;

    key = ftok("./31.c", 311);
    binarySemId = semget(key, 1, IPC_CREAT | 0777);
    arg.val = 1;
    int status = semctl(binarySemId, 0, SETVAL, arg);
    if(status == -1){
        printf("%s\n",strerror(errno));
    }
    else{
        printf("Binary semaphore has been created with ID: %d\n", binarySemId);
    }

    key = ftok("./31.", 312);
    countingSemId = semget(key, 1, IPC_CREAT | 0777);
    arg.val = 5;
    status = semctl(countingSemId, 0, SETVAL, arg);
    if(status == -1){
        printf("%s\n",strerror(errno));
    }
    else{
        printf("Counting semaphore has been created with ID: %d\n", countingSemId);
    }
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./31
Binary semaphore has been created with ID: 0
Counting semaphore has been created with ID: 1

*/