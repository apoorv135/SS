/* 
MT2020013 Apoorv Panse
33. Write a program to communicate between two machines using socket.
This is a CLIENT Program.
*/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h> 

int main()
{
    struct sockaddr_in server;
    int sd, msgLen;
    char buff[50];

    sd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
        //server.sin_addr.s_addr = "IP";
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5555);

    printf("Waiting to connect...\n");
    connect(sd, (struct sockaddr *) &server, sizeof(server));
    read(sd, buff, sizeof(buff));
    printf("Message from server: %s", buff);
    printf("Enter a message: \n");
    scanf("%[^\n]", buff);
    write(sd, buff, sizeof(buff));
}

/*
apoorv@apoorv-GV62VR-7RF:/home/apoorv/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./33b
Waiting to connect...
Message from server: Connected to server...
Enter a message: 
Hello Server from Client 1

apoorv@apoorv-GV62VR-7RF:/home/apoorv/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./33b
Waiting to connect...
Message from server: Connected to server...
Enter a message: 
Hello Server from client 2

*/