#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include<signal.h>

#define PORT 55555

void signal_handler(int signum){
    signal(SIGINT, signal_handler);
    printf("\nLog Out from Server\n");
    fflush(stdout);
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in server;
    int socketDescriptor;
    char output[1024];
    char message[1024];
    char input[1024];
    signal(SIGINT, signal_handler);
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    connect(socketDescriptor, (void *)(&server), sizeof(server));
    write(socketDescriptor, "Connected", 10);
    while(1){
        read(socketDescriptor, output, 1024);
        write(socketDescriptor, "ACK", 4);
        if(strcmp("RW", output) == 0){
            read(socketDescriptor, message, sizeof(message));
            printf("%s", message);
            scanf("%s", input);
            write(socketDescriptor, input, sizeof(input));
        }
        else if(strcmp("R", output) == 0){
            read(socketDescriptor, message, sizeof(message));
            write(socketDescriptor, "ACK", 4);
            printf("%s\n", message);

        }
        else if(strcmp("CLOSE", output) == 0){
            shutdown(socketDescriptor, SHUT_WR);
            printf("Closing Connection\n");
            break;
        }
        memset(message, 0, sizeof(message));
        memset(output, 0, sizeof(output));
    }
    return 0;
}
