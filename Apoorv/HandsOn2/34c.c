/* 
MT2020013 Apoorv Panse
34. Write a program to create a concurrent server.
a. use fork
b. use pthread_create

THIS IS A CLIENT PROGRAM THAT COMMUNICATES WITH THE SERVER
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(){
    struct sockaddr_in server;
	int sfd,messageLength;
	char buffer[50];
	char result;
	  
	sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(5555);  
	
	write(1,"Waiting to connect...\n", sizeof("Waiting to connect...\n"));
	connect(sfd,(struct sockaddr *)&server,sizeof(server)); 
	
	read(sfd, buffer,sizeof(buffer));
	printf("%s\n",buffer);
	write(1,"Write to server:",sizeof("Write to server"));
	scanf(" %[^\n]",buffer);
	write(sfd,buffer,sizeof(buffer));  

	close(sfd);
    return 0;

}
/*
These were executed Concurrently from Multiple Terminals

apoorv@apoorv-GV62VR-7RF:/home/apoorv/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./34c
Waiting to connect...
Connected to server...

Write to server:Hello from client 1
apoorv@apoorv-GV62VR-7RF:/home/apoorv/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./34c
Waiting to connect...
Connected to server...

Write to server:Hello From client 2
apoorv@apoorv-GV62VR-7RF:/home/apoorv/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./34c
Waiting to connect...
Connected to server...

Write to server:Hello from client 3

*/