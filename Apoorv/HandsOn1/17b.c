/* 
MT2020013 Apoorv Panse
Write a program to simulate online ticket reservation. Implement write lock
a. Write a program to open a file, store a ticket number and exit. 
b. Write a separate program, to open the file, implement write lock, read the ticket number, increment the number and print
the new ticket number then close the file.
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd;
  struct {
    int ticket;
  } db;

  struct flock wLock;

  fd = open("test17.txt", O_RDWR);
  read(fd, &db, sizeof(db));

  printf("Getting write Lock...\n");

  wLock.l_type = F_WRLCK;
  wLock.l_whence = SEEK_CUR;
  wLock.l_start = 0;
  wLock.l_len = 0;
  wLock.l_pid = getpid();

  fcntl(fd, F_SETLKW, &wLock);

  printf("Inside the critical section\n");

  db.ticket++;

  printf("Your New Ticket number is: %d\n", db.ticket);
  lseek(fd, 0L, SEEK_SET);
  write(fd, &db, sizeof(db));

  printf("Press Enter to exit CS \n");
  getchar();
  wLock.l_type = F_UNLCK;

  printf("EXITED\n");
  fcntl(fd, F_SETLK, &wLock);
  printf("finished...\n");

  return 0;
}
