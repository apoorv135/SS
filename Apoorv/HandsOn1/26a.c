/*
MT2020013 Apoorv Panse
Write a program to execute an executable program.
a. use some executable program
b. pass some input to an executable program. (for example execute an executable of $./a.out name)
*/

#include <stdio.h>

int main(int argc, char const *argv[])
{   
    printf("\nPrinting the arguments passed : \n"); 
    for(int i=0;i<argc;i++){
        printf("%s\n",argv[i]);
    }
    return 0;
}

