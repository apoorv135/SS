/* 
MT2020013 Apoorv Panse
34. Write a program to create a concurrent server.
a. use fork
b. use pthread_create

THIS SERVER USES THREADS FOR CONCURRENT EXECUTION
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *handler(void *nsd)
{
	int nsfd=*(int*)nsd;
	char buffer[50];
	write(1,"Connected to client...\n",sizeof("Connected to client...\n"));
	write(nsfd,"Connected to server...\n",sizeof("Connected to server...\n"));
	read(nsfd, buffer,sizeof(buffer));
	printf("%s\n",buffer);

}

int main()
{
	struct sockaddr_in serverAddress,clientAddress;
	int sfd,nsd,clientLen;
	pthread_t threads;
	char buffer[50];	
	  
	sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
	
	serverAddress.sin_family=AF_INET;							
	serverAddress.sin_addr.s_addr=INADDR_ANY;
	serverAddress.sin_port=htons(5555);  						
	bind(sfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));    
	listen(sfd,5); 									
	printf("Waiting for client...\n");

	while(1)
	{
		clientLen=sizeof(clientAddress);
		nsd=accept(sfd,(struct sockaddr *)&clientAddress,&clientLen); 
		if(pthread_create(&threads,NULL,handler,(void*)&nsd)<0)
		{
			perror("Error");
			return 1;
		}
		
	}
	pthread_exit(NULL);
	close(sfd);
	return 0;
}
