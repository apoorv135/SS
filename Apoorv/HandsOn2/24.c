/* 
MT2020013 Apoorv Panse
24. Write a program to create a message queue and print the key and message queue id.
*/

#include<stdio.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>

int main()
{
  int messageId;
  key_t key;
  key=ftok("./24.c",100);
 
  messageId=msgget(key,IPC_CREAT|IPC_EXCL|0777);   
  if(messageId < 0)
  {	
  	printf("%s\n",strerror(errno));
    exit(0);
  	
  }
  
  printf("Key=%d \nKey in hex =0x%0x\nMessage queue id=%d\n",key,key,messageId);
  return 0;
}
/*
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ./a.out
Key=1677988634 
Key in hex =0x6404131a
Message queue id=1
apoorv@apoorv-GV62VR-7RF:~/UBUNTU_WORKSPACE/SS/Apoorv/HandsOn2$ ipcs -p

------ Message Queues PIDs --------
msqid      owner      lspid      lrpid     
0        apoorv          0         0
1        apoorv          0         0

------ Shared Memory Creator/Last-op PIDs --------
shmid      owner      cpid       lpid      
32769      apoorv     3404       1642      
6          apoorv     2274       1642      
32783      apoorv     3618       1642      
16         apoorv     3064       1642      
17         apoorv     3064       1642      
18         apoorv     1955       1642      
32787      apoorv     3618       1642      
20         apoorv     1828       4646      
21         apoorv     3118       1642      
22         apoorv     3118       1642      
23         apoorv     3118       1642      
24         apoorv     3118       1642      
32793      apoorv     3618       1642      
27         apoorv     2099       1642      
28         apoorv     1955       1642      
31         apoorv     3404       3410      
33         apoorv     2099       1642  
*/