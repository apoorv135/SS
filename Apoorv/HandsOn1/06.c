/* 
MT2020013 Apoorv Panse
Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
*/
#include <stdio.h>
#include <fcntl.h>    
#include <unistd.h>
#include <string.h>

int main() {
    char buffer[100];
    char message1[]="Please enter something : ";
    write(1, message1, sizeof(message1));
    int numberOfChars;
    numberOfChars=read(0, buffer, sizeof(buffer));
    char message2[]="You have entered : ";
    write(1, message2, sizeof(message2));
    write(1, buffer, numberOfChars);
    return 0;
}