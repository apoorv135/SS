/*
MT2020013 Apoorv Panse
Write a program to execute an executable program.
Write a program to execute ls -Rl by the following system calls
a. execl
b. execlp
c. execle
d. execv
e. execvp
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    char* const args[] = {"ls", "-Rl", (char *) NULL};
    printf("Executing ls -Rl using execv()\n");
	execv("/bin/ls", args);
    return 0;
}