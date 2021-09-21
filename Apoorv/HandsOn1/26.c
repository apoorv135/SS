/*
MT2020013 Apoorv Panse
Write a program to execute an executable program.
a. use some executable program
b. pass some input to an executable program. (for example execute an executable of $./a.out name)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main()
{   //test file execution should print arguments 'Hello' and 'World' that is passed...
    //Please compile test26.c first and name the executable to test26.out
    int x = execl("./test26.out", "Hello","World", (char *) NULL);
    if ( x < 0 ){
        printf("%s\n",strerror(errno));
    }
    
	return 0;
}

/*
Printing the arguments passed : 
Hello
World
*/