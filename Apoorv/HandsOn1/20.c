/* 
MT2020013 Apoorv Panse
Find out the priority of your running program. Modify the priority with nice command.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {

    int current_priority = getpriority(PRIO_PROCESS, 0);

	printf("Priority of the current process: %d\n", current_priority);
	
	nice(2);

	current_priority = getpriority(PRIO_PROCESS, 0);
	
	printf("Priority after nice function call: %d\n", current_priority);

     if(setpriority(PRIO_PROCESS, 0, 4) < 0){
        printf("%s\n",strerror(errno));
    }
	
	current_priority = getpriority(PRIO_PROCESS, 0);
	
	printf("Priority after setpriority function call: %d\n", current_priority);
        
    return 0;
}