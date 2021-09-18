/* 
MT2020013 Apoorv Panse
Write a program to find out time taken to execute getpid system call. Use time stamp counter.
*/

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>    
#include <unistd.h>
#include <time.h>

int main() {

    clock_t time = clock();
    getpid();
    time = clock() - time;
    double seconds = ((double)time)/CLOCKS_PER_SEC;
    printf("Time taken for getpid(): %f", seconds);    
    return 0;
}