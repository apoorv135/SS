/*
MT2020013 Apoorv Panse
Write a program to get maximum and minimum real time priority.
*/

#include <stdio.h>
#include <sched.h>
#include <stdlib.h>

int main()
{	

	printf("Min priority: %d\n", sched_get_priority_min(SCHED_FIFO));	
	printf("Max priority: %d\n", sched_get_priority_max(SCHED_FIFO));

	return 0;
}