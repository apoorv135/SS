/* 
MT2020013 Apoorv Panse
34. Write a program to create a concurrent server.
a. use fork
b. use pthread_create

THIS SERVER USES FORK FOR CONCURRENT EXECUTION
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(){
    
    struct sockaddr_in serverAddress,clientAddress;
	int sfd,nsd,clientLen;
	char buffer[50];
	  
	sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
	
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=INADDR_ANY;
	serverAddress.sin_port=htons(5555);  						
	bind(sfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));    
	listen(sfd,2); 									

	printf("Waiting for client...\n");
	while(1)
	{
		clientLen=sizeof(clientAddress);
		nsd=accept(sfd,(struct sockaddr *)&clientAddress,&clientLen);  
		printf("\n");
		write(1,"Connected to client...\n",sizeof("Connected to client...\n"));
		
		if(!fork()){
			close(sfd);
			write(nsd,"Connected to server...\n",sizeof("Connected to server...\n"));
			read(nsd, buffer,sizeof(buffer));
			printf("%s\n",buffer);
			exit(0);
		}
		else
		{
			close(nsd);
		}
	}
	close(sfd);

    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./34a
Waiting for client...

Connected to client...
Hello from client 1

Connected to client...

Connected to client...
Hello from client 3
Hello From client 2

*/