/*
MT2020013 Apoorv Panse
Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,
SCHED_RR).
*/


#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
 
int main()
{  
	int pid = getpid();
	printf("Before changing the policy process is using ");
    int policyID = sched_getscheduler(pid);
	printf(": %d\n", policyID);

	struct sched_param param;
	param.sched_priority = 1;   
	sched_setscheduler(pid, SCHED_RR, &param) ;

    policyID = sched_getscheduler(pid);
	printf("After changing the policy process is using : %d\n", policyID);

	return 0;
}