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
#include <string.h>
#include <errno.h>

union semun{
    int value;
    struct semid_ds *buffer;
    unsigned short int *array;
};

int main() {

	struct ticket
	{
	    int ticket_number;
	}db;

	db.ticket_number = 0;

    union semun arg;
    int key, semaphoreID;

    key = ftok("./32a.c", 321);
    semaphoreID = semget(key, 1, 0);
    if(semaphoreID==-1){			
        semaphoreID = semget(key, 1, IPC_CREAT|0777);    
        arg.value = 1;
        semctl(semaphoreID, 0, SETVAL, arg);
    }
    
    printf("Before critial section..\n");

    struct sembuf sops = {0, -1, 0};  
    
    if(semop(semaphoreID, &sops, 1) < 0){
        printf("%s\n",strerror(errno));
        exit(0);
    }
    printf("Inside the critial section..\n");
	
	int fd = open("record.txt", O_CREAT|O_EXCL|O_RDWR, 0777);
    if(fd == -1){
    	db.ticket_number = 0;
    	write(fd, &db, sizeof(struct ticket));
    }
    close(fd);

    fd = open("record.txt", O_RDWR);
    
    printf("In the critical section.\n");

	if(read(fd, &db, sizeof(db)) < 0){
        printf("%s\n",strerror(errno));
        exit(0);
    }
	
	db.ticket_number++;

	lseek(fd, 0, SEEK_SET);
	write(fd, &db, sizeof(struct ticket));
	
	printf("New ticket number: %d\n", db.ticket_number);
	close(fd);

    printf("Press any key to exit the lock.\n");
	getchar();    

    sops.sem_op = 1;
    if(semop(semaphoreID, &sops, 1) < 0){
        printf("%s\n",strerror(errno));
        exit(0);
    }
    printf("After the critial section\n");
    return 0;
    
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./32a
Before critial section..
Inside the critial section..
In the critical section.
New ticket number: 1
Press any key to exit the lock.

After the critial section
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./32a
Before critial section..
Inside the critial section..
In the critical section.
New ticket number: 2
Press any key to exit the lock.

After the critial section

*/