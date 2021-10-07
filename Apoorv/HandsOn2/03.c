/* 
MT2020013 Apoorv Panse
3. Write a program to set (any one) system resource limit. Use setrlimit system call.
*/

#include<stdio.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<string.h>

int main()
{
	struct rlimit rlim1;
	struct rlimit rlim2;
	getrlimit(RLIMIT_NOFILE,&rlim1);
	printf("\nDefault values of number of FDs :%ld\n", rlim1.rlim_cur);
	rlim1.rlim_cur=4096;
	setrlimit(RLIMIT_NOFILE,&rlim1);
	getrlimit(RLIMIT_NOFILE, &rlim2);
	printf("Values of number of FDs after changing:%ld\n\n", rlim2.rlim_cur);
	return 0;

}
/*OUTPUT : 
Default values of number of FDs :8192
Values of number of FDs after changing:4096
*/