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

int main(int agrc, int const argv[]){
	
	key_t sem_key = ftok("./32d.c", 324);                    
	
	
	int id = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | 0777);
	int i = semctl(id, 0, SETVAL, 1);
	
	printf("Semaphore ID : %d\n", id);
				
	if(semctl(id, 0, IPC_RMID, 1) < 0){
        printf("%s\n",strerror(errno));
        exit(0);
    }
    else 
        printf("Semaphore removed\n");
	
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./32d
Semaphore ID : 4
Semaphore removed
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     

apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ 

*/