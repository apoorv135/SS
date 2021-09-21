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

  struct 
  {
    int ticket_no;
  } db;

  int ticket;

  printf("Enter the TicketNumber: ");  
  scanf("%d",&ticket);
  db.ticket_no = ticket;

  fd = open("test17.txt", O_CREAT | O_RDWR, 0777);
  write(fd, &db, sizeof(db));
  close(fd);
  

  return 0;
}
