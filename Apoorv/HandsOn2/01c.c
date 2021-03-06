/* 
MT2020013 Apoorv Panse
1. Write a separate program (for each time domain) to set a interval timer in 10sec and
10micro second
a. ITIMER_REAL
b. ITIMER_VIRTUAL
c. ITIMER_PROF
*/
#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void handler_function()
{
	printf("SIGPROF signal is caught \n");
	printf("Timer Stopped \n");
	exit(0);

}

int main()
{
	signal(SIGPROF, handler_function);
	struct timeval tv={10,10}; 
	struct timeval tv1={0,0};  
	
	struct itimerval val; 
	val.it_value=tv;
	val.it_interval=tv1;
	
	printf("Start timer\n");
	setitimer(ITIMER_PROF, &val, NULL); 
	sleep(5);
	while(1){}
	return 0;

}
/* 
Start timer
SIGPROF signal is caught
Timer Stopped
*/