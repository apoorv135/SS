#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "Structures.c"

void initial_setup(){
    if(open("login.dat", O_RDONLY) == -1){
        
        creat("login.dat", 0644);
        int fd = open("login.dat", O_RDWR);
        struct AccountDetails admin;
        strcpy(admin.username, "admin");
        strcpy(admin.password, "admin");
        admin.accountType = 3;
        admin.sessionFlag = 0;
        strcpy(admin.accountStatus, "ACTIVE");
        write(fd, &admin, sizeof(admin));
        close(fd);
    }
    if(open("train.dat", O_RDONLY) == -1){
        
        creat("train.dat", 0644);
    }
    if(open("booking.dat", O_RDONLY) == -1){
        
        creat("booking.dat", 0644);
    }
    if(open("bid.dat", O_RDONLY) == -1){
        
        creat("bid.dat", 0644);
        int fd = open("bid.dat", O_RDWR);
        int bid = 1;
        write(fd, &bid, sizeof(bid));
        close(fd);
    }
}