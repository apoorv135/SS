/* 
MT2020013 Apoorv Panse
33. Write a program to communicate between two machines using socket.
This is a SERVER Program.
*/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h> 

int main()
{
    struct sockaddr_in server, client;
    int sd, nsd, clientLen;
    char buff[50];

    sd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5555);

    bind(sd, (struct sockaddr *) &server, sizeof server);
    listen(sd, 2);

    while(1){
            printf("\nWaiting for client...\n");
            clientLen = sizeof(client);
            nsd = accept(sd, (struct sockaddr*) &client, &clientLen);
            printf("Connecting to client...\nA new is client connected\n");
            write(nsd, "Connected to server...\n", sizeof("Connected to server...\n"));
            read(nsd, buff, sizeof(buff));
            printf("Message from client: %s", buff);
            close(nsd);
    }
    close(sd);
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./33a

Waiting for client...
Connecting to client...
A new is client connected
Message from client: Hello Server from Client 1
Waiting for client...
Connecting to client...
A new is client connected
Message from client: Hello Server from client 2
Waiting for client...
^C

*/