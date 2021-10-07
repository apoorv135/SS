/* 
MT2020013 Apoorv Panse
4. Write a program to measure how much time is taken to execute 100 getppid ( )
system call. Use time stamp counter.
*/

#include<sys/time.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

unsigned long long rdtsc(void)
{
	unsigned long long dst;
	__asm__ __volatile("rdtsc" : "=A"(dst));
	return dst;
}

int main()
{
	long long int start, end;
	start=rdtsc();
	int i=0;
	printf("Initial value=%llu\n",start);
	while(i<100)
	{
		getppid();
        ++i;
	}
	end=rdtsc();
	printf("End Value= %llu\n",end);
	end=end-start;
	printf("Cycles taken= %llu\n",end);
	return 0;
}
/*
Initial value=2110912487
End Value= 2111420794
Cycles taken= 508307
*/