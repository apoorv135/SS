/*
MT2020013 Apoorv Panse
27. Write a program to receive messages from the message queue.
a. with 0 as a flag
b. with IPC_NOWAIT as a flag
*/
/* 
MT2020013 Apoorv Panse
26. Write a program to send messages to the message queue. Check $ipcs -q
*/

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

struct msg
{
  long int messageType;
  char messageText[1024];
}myq;

int main()
{
    int messageId;
    key_t key;

    key=ftok("./27.c",27);
    messageId=msgget(key,IPC_CREAT|0777);    

    myq.messageType = 1;
    strncpy(myq.messageText, "HelloWorld", 1024);
    msgsnd(messageId, &myq, sizeof myq, 0);

    msgrcv(messageId, &myq, sizeof myq, 1, 0);
    
    printf("Part a with 0 as a flag\n");
    printf("Type: %ld\nMessage: %s\n", myq.messageType, myq.messageText);

    msgrcv(messageId, &myq, sizeof myq, 22, IPC_NOWAIT);
    printf("Part b with IPC_NOWAIT as a flag\n");
    printf("Type: %ld\nMessage: %s\n", myq.messageType, myq.messageText);
    
    return 0;
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Part a with 0 as a flag
Type: 1
Message: HelloWorld
Part b with IPC_NOWAIT as a flag
Type: 1
Message: HelloWorld

*/