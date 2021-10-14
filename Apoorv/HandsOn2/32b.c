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

    int key=ftok("./32b.c", 322);

    int semaphoreID = semget(key, 1, 0);
    if(semaphoreID == -1){
    	semaphoreID = semget(key, 1, IPC_CREAT|0777);    
        arg.value = 1;
        semctl(semaphoreID, 0, SETVAL, arg);
    }

    struct sembuf buf = {0,-1,0};
    
    printf("Before Entering into the critical section..\n");
    if(semop(semaphoreID, &buf, 1) < 0){
        printf("%s\n",strerror(errno));
        exit(0);
    }

    int shmid = shmget(key, 1024, 0);
    if(shmid == -1){
        shmid = shmget(key, 1024, IPC_CREAT|0777);    
    }
    printf("Inside the critical section\n");

    char *ptr = shmat(shmid, (void *)0, 0);

    printf("Enter data to write into shared memory: ");
    scanf(" %[^\n]", ptr);
    getchar();
    buf.sem_op=1;
    printf("Press enter to exit the lock.\n");
	getchar();
    semop(semaphoreID, &buf, 1);
   
	return 0;	
    
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./32b
Before Entering into the critical section..
Inside the critical section
Enter data to write into shared memory: Data1
Press enter to exit the lock.

apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ipcs -m

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
0x42041316 131073     apoorv     777        1024       0                       
0x00000000 32776      apoorv     600        524288     2          dest         
0x00000000 9          apoorv     600        524288     2          dest         
0x00000000 98315      apoorv     600        524288     2          dest         
0x00000000 12         apoorv     600        524288     2          dest         
0x00000000 15         apoorv     600        4194304    2          dest         
0x00000000 16         apoorv     600        524288     2          dest         
0x00000000 19         apoorv     600        524288     2          dest         
0x00000000 26         apoorv     600        524288     2          dest         
0x00000000 27         apoorv     600        67108864   2          dest         
0x00000000 28         apoorv     606        10920960   2          dest         
0x00000000 29         apoorv     606        10920960   2          dest         
0x00000000 30         apoorv     606        2880000    2          dest         
0x00000000 31         apoorv     606        2880000    2          dest         
0x00000000 34         apoorv     600        524288     2          dest         
0x00000000 35         apoorv     600        4194304    2          dest         
0x00000000 36         apoorv     600        67108864   2          dest         
0x00000000 65578      apoorv     600        524288     2          dest      

*/