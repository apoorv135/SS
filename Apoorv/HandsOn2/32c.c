/* 
32. Write a program to implement semaphore to protect any critical section.
a. rewrite the ticket number creation program using semaphore
b. protect shared memory from concurrent write access
c. protect multiple pseudo resources ( may be two) using counting semaphore
d. remove the created semaphore
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

union semun{
    int value;
    struct semid_ds *buffer;
    unsigned short int *array;
};

int main() {

  union semun arg;

    int key=ftok("./32c.c", 323);

    int semaphoreID = semget(key, 1, 0);
    if(semaphoreID == -1){
    	semaphoreID = semget(key, 1, IPC_CREAT|0644);    
        arg.value = 2;
        semctl(semaphoreID, 0, SETVAL, arg);
    }

    struct sembuf buf = {0,-1,0};
    
    printf("before Entering into the critical section..\n");
    printf("Waiting for the unlock...\n");
    
    if(semop(semaphoreID, &buf, 1) < 0){
        printf("%s\n",strerror(errno));
        exit(0);
    }
    int shmid = shmget(key, 1024, 0);
    if(shmid == -1){
        shmid = shmget(key, 1024, IPC_CREAT|0744);    
    }
    printf("In the critical section.\n");

    char *ptr = shmat(shmid, (void *)0, 0);

    printf("Write some data to the shared memory: ");
    scanf(" %[^\n]", ptr);

    buf.sem_op=1;
    semop(semaphoreID, &buf, 1);

    printf("Press any key to exit the lock.\n");
	getchar();
	return 0;	
    
}

/*

*/