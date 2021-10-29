#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "Structures.c"

void initial_setup(){
    if(open("loginInfo.dat", O_RDONLY) == -1){
        
        creat("loginInfo.dat", 0644);
        int fd = open("loginInfo.dat", O_RDWR);
        struct AccountDetails admin;
        strcpy(admin.username, "admin");
        strcpy(admin.password, "admin");
        admin.accountType = 3;
        admin.sessionFlag = 0;
        strcpy(admin.accountStatus, "ACTIVE");
        write(fd, &admin, sizeof(admin));
        close(fd);
    }
    if(open("trainInfo.dat", O_RDONLY) == -1){
        
        creat("trainInfo.dat", 0644);
    }
    if(open("bookingInfo.dat", O_RDONLY) == -1){
        
        creat("bookingInfo.dat", 0644);
    }
    if(open("bidInfo.dat", O_RDONLY) == -1){
        
        creat("bidInfo.dat", 0644);
        int fd = open("bidInfo.dat", O_RDWR);
        int bid = 1;
        write(fd, &bid, sizeof(bid));
        close(fd);
    }
}