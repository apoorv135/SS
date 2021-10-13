/* 
MT2020013 Apoorv Panse
30. Write a program to create a shared memory.
a. write some data to the shared memory
b. attach with O_RDONLY and check whether you are able to overwrite.
c. detach the shared memory
d. remove the shared memory
*/

#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdio.h>

int main()
{
    int shmid;
    int key;    
    key = ftok("./30.c", 30);

    printf("Creating shared memory.\n");
    shmid = shmget(key, 1024, IPC_CREAT|0777);
    
    printf("Attaching shared memory.\n");
    char *writeData = shmat(shmid, 0, 0);    

    printf("Enter some data: ");
    scanf("%[^\n]", writeData);

    char * readData = shmat(shmid, 0, SHM_RDONLY);
    printf("Reading data from shared memory: %s\n", readData);

    printf("Deattaching shared memory.\n");
    shmdt((void *)readData);

    printf("Destroying shared memory.\n");
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./30
Creating shared memory.
Attaching shared memory.
Enter some data: Hello Apoorv
Reading data from shared memory: Hello Apoorv
Deattaching shared memory.
Destroying shared memory.

*/