/* 
MT2020013 Apoorv Panse
6. Write a simple program to create three threads.
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

void * threadDemo(void *i)
{
	printf("Inside Thread \n");
	return 0;
}
void main()
{
	pthread_t thread;
	for(int i=0;i<3;i++)
	{
		
		pthread_create(&thread, NULL, threadDemo,NULL);		
		printf("Running thread %d\n",i);
	
	}
	pthread_exit(NULL);

}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ gcc -pthread -o 06 06.c
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./06
Running thread 0
Inside Thread 
Running thread 1
Inside Thread 
Running thread 2
Inside Thread 
*/