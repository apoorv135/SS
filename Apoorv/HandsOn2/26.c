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

void main()
{
    int messageId;
    key_t key;

    key=ftok("./26.c",26);
    messageId=msgget(key,IPC_CREAT|0777);    
  
    myq.messageType = 1;
    strncpy(myq.messageText, "HelloWorld", 1024);
    msgsnd(messageId, &myq, sizeof myq, 0);
  
}

/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x6104131a 0          apoorv     777        0            0           
0x6404131a 1          apoorv     777        0            0           
0x64040d8f 2          apoorv     777        0            0           
0x1a04131c 3          apoorv     777        1032         1           
0x1b041320 4          apoorv     777        0            0           
0xffffffff 5          apoorv     777        0            0  

*/