/* 
MT2020013 Apoorv Panse
2. Write a program to print the system resource limits. Use getrlimit system call.
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/resource.h>
#include<sys/time.h>
#include<unistd.h>

int main()
{
	struct rlimit limits;
	char rlimits[18][20]={"RLIMIT_CPU", "RLIMIT_CORE", "RLIMIT_FSIZE","RLIMIT_NOFILE","RLIMIT_NPROC","RLIMIT_AS","RLIMIT_DATA","RLIMIT_LOCKS","RLIMIT_MEMLOCK","RLIMIT_MSGQUEUE", "RLIMIT_NICE","RLIMIT_NOFILE","RLIMIT_NPROC","RLIMIT_RSS","RLIMIT_RTPRIO","RLIMIT_RTTIME","RLIMIT_SIGPENDING","RLIMIT_STACK"};
	int irlimits[18]={RLIMIT_CPU, RLIMIT_CORE,RLIMIT_FSIZE,RLIMIT_NOFILE,RLIMIT_NPROC,RLIMIT_AS,RLIMIT_DATA,RLIMIT_LOCKS,RLIMIT_MEMLOCK,RLIMIT_MSGQUEUE, RLIMIT_NICE,RLIMIT_NOFILE,RLIMIT_NPROC,RLIMIT_RSS,RLIMIT_RTPRIO,RLIMIT_RTTIME,RLIMIT_SIGPENDING,RLIMIT_STACK};
	for(int i=0;i<18;i++)
	{
	getrlimit(irlimits[i],&limits);
	printf("%s :-\nsoftlimit: %lu\nHard limit: %lu\n\n",rlimits[i],limits.rlim_cur,limits.rlim_max);
	
	}
}

/*
RLIMIT_CPU :-
softlimit: 18446744073709551615
Hard limit: 18446744073709551615

RLIMIT_CORE :-
softlimit: 0
Hard limit: 18446744073709551615

RLIMIT_FSIZE :-
softlimit: 18446744073709551615
Hard limit: 18446744073709551615

RLIMIT_NOFILE :-
softlimit: 8192
Hard limit: 1048576

RLIMIT_NPROC :-
softlimit: 63291
Hard limit: 63291

RLIMIT_AS :-
softlimit: 18446744073709551615
Hard limit: 18446744073709551615

RLIMIT_DATA :-
softlimit: 18446744073709551615
Hard limit: 18446744073709551615

RLIMIT_LOCKS :-
softlimit: 18446744073709551615
Hard limit: 18446744073709551615

RLIMIT_MEMLOCK :-
softlimit: 2082528768
Hard limit: 2082528768

RLIMIT_MSGQUEUE :-
softlimit: 819200
Hard limit: 819200

RLIMIT_NICE :-
softlimit: 0
Hard limit: 0

RLIMIT_NOFILE :-
softlimit: 8192
Hard limit: 1048576

RLIMIT_NPROC :-
softlimit: 63291
Hard limit: 63291

RLIMIT_RSS :-
softlimit: 18446744073709551615
Hard limit: 18446744073709551615

RLIMIT_RTPRIO :-
softlimit: 0
Hard limit: 0

RLIMIT_RTTIME :-
softlimit: 18446744073709551615
Hard limit: 18446744073709551615

RLIMIT_SIGPENDING :-
softlimit: 63291
Hard limit: 63291

RLIMIT_STACK :-
softlimit: 8388608
Hard limit: 18446744073709551615
*/