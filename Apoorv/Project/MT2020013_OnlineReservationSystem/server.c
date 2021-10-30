
#include "Structures.c"
#include "utilityFunctions.c"

#define PORT 55555


int main(int argc, char const *argv[]){
    struct sockaddr_in server, client;
    int socket_desc, size_client, client_desc;
    initial_setup();
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    
    server.sin_port = htons(PORT);
    if(bind(socket_desc, (void *)(&server), sizeof(server)) < 0) {
	    perror("Error on binding:");
	    exit(EXIT_FAILURE);
	}
    listen(socket_desc, 5);
    while(1){
        size_client = sizeof(client);
	    if((client_desc = accept(socket_desc, (struct sockaddr*)&client, &size_client)) < 0) {
	        printf("Error on accept.\n");
	        exit(EXIT_FAILURE);
	    }
        if(fork() == 0){
            clientConn(client_desc);
            close(client_desc);
            exit(EXIT_SUCCESS);
        }else{
            close(client_desc);
        }
    }
    return 0;
}
