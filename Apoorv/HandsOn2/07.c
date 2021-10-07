/* 
MT2020013 Apoorv Panse
7. Write a simple program to print the created thread ids.
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

void * threadDemo(void *i)
{
	printf("TID: %ld\n\n", pthread_self());
	return 0;
}
void main()
{
	pthread_t thread;
	for(int i=1;i<=3;i++)
	{		
		pthread_create(&thread, NULL, threadDemo,(void*)&i);	
	}
	pthread_exit(NULL);

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ gcc -pthread -o 07 07.c
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./07
TID: 139896088176192

TID: 139896079783488

TID: 139896071390784
*/