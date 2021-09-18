/* 
MT2020013 Apoorv Panse
Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to
verify whether the data is available within 10 seconds or not (check in $man 2 select).
*/

#include <sys/select.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>    
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(){
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    struct timeval time;
    time.tv_sec = 10;
    time.tv_usec = 0;

    int return_val = select(1, &rfds, NULL, NULL, &time);

    if (return_val < 0){
        printf("%s\n",strerror(errno));
    }        
    else if (return_val){
        printf("Data has been entered within specified time\n");
    }
    else
        printf("No data available within specified time.\n");

    return 0;
}
