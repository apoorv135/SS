/* MT2020013 APOORV PANSE
This is main server code. The code is divided into modules for ease of understanding. 
Server keeps listening for client connections and forks a separate session for each client.
I have assumed that server is forever online serving the client requests until the machine is on.
*/

#include "Structures.c"
#include "utilityFunctions.c"

#define PORT 55555


int main(int argc, char const *argv[]){
    struct sockaddr_in server, client;
    int socketDescriptor, size_client, client_desc;
    initial_setup();
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;    
    server.sin_port = htons(PORT);

    if(bind(socketDescriptor, (void *)(&server), sizeof(server)) < 0) {
	    perror("Error on binding:");
	    exit(EXIT_FAILURE);
	}
    printf("Listening Incoming request on port %d... \n", PORT);
    listen(socketDescriptor, 5);
    
    while(1){
        size_client = sizeof(client);
        
	    if((client_desc = accept(socketDescriptor, (struct sockaddr*)&client, &size_client)) < 0) {
	        printf("Error on accept.\n");
	        exit(EXIT_FAILURE);
	    }
        if(fork() == 0){
            
            clientConn(client_desc);
            close(client_desc);
            printf("Client has disconnected...\n");
            printf("Listening Incoming request on port %d... \n", PORT);
            exit(EXIT_SUCCESS);
        }else{
            close(client_desc);
        }
    }
    return 0;
}
