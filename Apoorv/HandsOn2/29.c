
/* 
MT2020013 Apoorv Panse
29. Write a program to remove the message queue.
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

    key=ftok("./28.c",28);
    messageId=msgget(key,IPC_CREAT|0777);    
    int status = msgctl(messageId, IPC_RMID, 0);    
    return 0;
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
0x1b040909 6          apoorv     777        2064         2           
0x1c040909 7          apoorv     777        0            0           

apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ gcc 29.c
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x6104131a 0          apoorv     777        0            0           
0x6404131a 1          apoorv     777        0            0           
0x64040d8f 2          apoorv     777        0            0           
0x1a04131c 3          apoorv     777        1032         1           
0x1b041320 4          apoorv     777        0            0           
0xffffffff 5          apoorv     777        0            0           
0x1b040909 6          apoorv     777        2064         2

msqid 7 has been removed
*/