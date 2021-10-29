#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


struct flock readLock(struct flock lock, int fd){
    lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	//printf("Before Entering the ReadLock critical section\n");
	fcntl(fd, F_SETLKW, &lock);
    //printf("Inside CS\n");
    return lock;
}

struct flock file_read_unlock(struct flock lock, int fd){
    lock.l_type = F_UNLCK;
	//printf("Unlocked\n");
	fcntl(fd, F_SETLK, &lock);
    return lock;
}

struct flock file_write_lock(struct flock lock, int fd){
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	//printf("Entering the WriteLock critical section\n");
	fcntl(fd, F_SETLKW, &lock);
    //printf("Inside CS \n");
    return lock;
}

struct flock file_write_unlock(struct flock lock, int fd){
    lock.l_type = F_UNLCK;
	// printf("Unlocked\n");
	fcntl(fd, F_SETLK, &lock);
    return lock;
}

void send_message(int desc, char *msg, char *input){
    for (size_t i = 0; i < 10000; i++);    
    write(desc, msg, 1024);
    read(desc, input, 1024);
}